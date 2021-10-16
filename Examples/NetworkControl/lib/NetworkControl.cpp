#include "NetworkControl.h"
#include "WifiScanResults.h"
#include "Toolbox.h"

using namespace Toolbox;
using namespace std;

NetworkControl::WpaStationState::WpaStationState()
   : connectionStatus(false)
   , interfaceName("wlan0")
   , rssiMeasurementRequired(false)
{
}

NetworkControl::NetworkControl(WpaControlInterface &wpaControl,  DhcpInterface &dhcp)
   : m_wpaControl(wpaControl)
   , m_dhcp(dhcp)
{
}

bool NetworkControl::Connect(string ssid, string passphrase)
{
   char buffer[CommandBufferLen];

   snprintf(buffer, CommandBufferLen, R"(SET_NETWORK 0 ssid "%s")", ssid.c_str());
   auto responseSSID = m_wpaControl.WpaCommand(string(buffer));
   
   snprintf(buffer, CommandBufferLen, R"(SET_NETWORK 0 psk "%s")", passphrase.c_str());
   auto responsePsk = m_wpaControl.WpaCommand(string(buffer));

   auto saveConfig = m_wpaControl.WpaCommand("SAVE_CONFIG");
   
   auto enableNetwork = m_wpaControl.WpaCommand("ENABLE_NETWORK 0");

   auto reconnect = m_wpaControl.WpaCommand("RECONNECT");

   return (responseSSID.outcome == Outcome::Success) && 
          (responsePsk.outcome == Outcome::Success) &&
          (saveConfig.outcome == Outcome::Success)   &&
          (enableNetwork.outcome == Outcome::Success) &&
          (reconnect.outcome == Outcome::Success);
}

bool NetworkControl::Disconnect()
{
   auto disconnect = m_wpaControl.WpaCommand("DISCONNECT");
   return disconnect.outcome == Outcome::Success;
}

bool NetworkControl::Reconnect()
{
   auto reconnect = m_wpaControl.WpaCommand("RECONNECT");
   return reconnect.outcome == Outcome::Success;
}

bool NetworkControl::ReconfigureStation()
{
   auto reconfigure = m_wpaControl.WpaCommand("RECONFIGURE");
   return reconfigure.outcome == Outcome::Success;
}

bool NetworkControl::GetAccessPointState(string &accessPointState)
{
   auto status = m_wpaControl.HostApdCommand("STATUS");

   if (status.outcome != Outcome::Success)
   {
      return false;
   }
   
   auto statusLines = ToVectorString(status.reply);
   auto statusParameter = GetParameter("state", statusLines);
   
   if (statusParameter != "")
   {
      accessPointState = statusParameter;
      return true;
   }
   return false;
}

bool NetworkControl::DisableAccessPointIfEnabled()
{
   string accessPointState;
   GetAccessPointState(accessPointState);
   
   if (accessPointState == "ENABLED")
   {
      return DisableAccessPoint();
   }
   if(accessPointState == "DISABLED")
   {
      return true;
   }
   
   return false;
}

bool NetworkControl::Initialise()
{
   HandleDhcpChangeAndNotify();
   return DisableAccessPointIfEnabled();
}

bool NetworkControl::EnableOpenAccessPoint(const string &SSID, u8 channel)
{
   DisableAccessPointIfEnabled();
   
   auto allowBroadcast = m_wpaControl.HostApdCommand("SET ignore_broadcast_ssid 0");
   auto setSSID = SetAccessPointSSID(SSID);
   auto setChannel = SetAccessPointChannel(channel);
   auto setState = SetAccessPointState(true);
   
   string accessPointState;
   GetAccessPointState(accessPointState);

   return allowBroadcast.outcome == Outcome::Success && 
          setSSID && 
          setChannel && 
          setState &&
          accessPointState == "ENABLED";
}

bool NetworkControl::EnableSecureAccessPoint(const string &SSID, const string &password, u8 channel)
{
   DisableAccessPointIfEnabled();
   
   auto allowBroadcast = m_wpaControl.HostApdCommand("SET ignore_broadcast_ssid 0");
   auto setSSID = SetAccessPointSSID(SSID);
   auto setChannel = SetAccessPointChannel(channel);
   auto security = SetAccessPointSecurity(password);
   auto setState = SetAccessPointState(true);
   
   string accessPointState;
   GetAccessPointState(accessPointState);

   return allowBroadcast.outcome == Outcome::Success && 
          setSSID && 
          setChannel && 
          setState &&
          security &&
          accessPointState == "ENABLED";
}

bool NetworkControl::DisableAccessPoint()
{
   return SetAccessPointState(false);
}

bool NetworkControl::SetAccessPointState(bool enabled)
{
   string stateCommand      = enabled ? "ENABLE"  : "DISABLE";
   string desiredFinalState = enabled ? "ENABLED" : "DISABLED";
   
   auto status = m_wpaControl.HostApdCommand(stateCommand);

   string actualState;
   GetAccessPointState(actualState);
   
   return status.outcome == Outcome::Success && 
          actualState == desiredFinalState;
}

bool NetworkControl::SetAccessPointSSID(string ssid)
{
   char command[CommandBufferLen];
   snprintf(command, CommandBufferLen, "SET ssid %s", ssid.c_str());

   auto responseSSID = m_wpaControl.HostApdCommand(string(command));
   return responseSSID.outcome == Outcome::Success;
}

bool NetworkControl::SetAccessPointSecurity(string password)
{
   char command[CommandBufferLen];
   
   snprintf(command, CommandBufferLen, "SET wpa_passphrase %s", password.c_str());
   auto responseSSID = m_wpaControl.HostApdCommand(string(command));

   auto keyManagement = m_wpaControl.HostApdCommand("SET wpa_key_mgmt WPA-PSK");
   auto rsn = m_wpaControl.HostApdCommand("SET rsn_pairwise CCMP");
   auto wpa= m_wpaControl.HostApdCommand("SET wpa 2");
   

   return (responseSSID.outcome == Outcome::Success) &&
          (keyManagement.outcome == Outcome::Success) &&
          (rsn.outcome == Outcome::Success) &&
          (wpa.outcome == Outcome::Success);
}

bool NetworkControl::SetAccessPointChannel(u8 channel)
{
   char command[CommandBufferLen];
   auto val = ANY_CHANNEL;
   
   if (channel == ANY_CHANNEL)
   {
      channel = 0;  // acs_survey (auto-select does not appear to work in current Alpine/hostpad Build)
   }
   
   snprintf(command, CommandBufferLen, "SET channel %d", channel);

   auto responseChannel = m_wpaControl.HostApdCommand(string(command));
   return responseChannel.outcome == Outcome::Success;
}

WpaControlResponse NetworkControl::HostapdRaw(string rawCommand)
{
   return m_wpaControl.HostApdCommand(rawCommand);
}

WpaControlResponse NetworkControl::WpaSupplicantRaw(string rawCommand)
{
   return  m_wpaControl.WpaCommand(rawCommand);
}

vector<WifiTypes::AccessPointInfo> NetworkControl::Scan(u8 channel)
{
   vector<WifiTypes::AccessPointInfo> scanResults;
   auto scan = m_wpaControl.WpaCommand("SCAN_RESULTS");

   if (scan.outcome != Outcome::Success)
   {
      return scanResults;
   }

   auto lines = ToVectorString(scan.reply, '\n');
   
   if (lines.size() > 1)
   {
      VectorString tableContents = {lines.begin() + 1, lines.end()};      // line 0 contains table header;
      scanResults = WifiScanResults::ToScanResults(tableContents, channel);
   }
   return scanResults;
}

bool NetworkControl::CheckForRssiMeasurementRequest()
{
   if (m_wpaStationState.rssiMeasurementRequired == false)
   {
      return true;
   }

   m_wpaStationState.rssiMeasurementRequired = false;

   if (m_rssiMeasurementSubscriber == nullptr)
   {
      return false;
   }

   auto measureRssi = m_wpaControl.WpaCommand("SIGNAL_POLL");
   if (measureRssi.outcome != Outcome::Success)
   {
      return false;
   }

   auto lines = ToVectorString(measureRssi.reply);
   if (lines.size() == 0)
   {
      return false;     
   }

   string averageRssi = GetParameter("AVG_RSSI", lines);
   if (averageRssi == "")
   {
      return false;
   }
 
   constexpr s32 conversionFailureDefault = -1000;
   auto rssi = ToInteger(averageRssi, conversionFailureDefault);
   if (rssi == conversionFailureDefault)
   {
      return false;
   }
   
   m_rssiMeasurementSubscriber(static_cast<s8>(rssi));
   return true;
}
   
bool NetworkControl::DoPeriodicActivity(bool accessPointScan)
{
   bool scanned;
   if (accessPointScan)
   {
      scanned = ScanAsync();
   }
   else
   {
      scanned = true;
   }

   auto wpa = CheckWpaStatus();
   auto dhcp = HandleDhcpChangeAndNotify();
   auto inspectStation = CheckForNewStationConnections();
   auto rssi = CheckForRssiMeasurementRequest();

   return scanned && wpa && dhcp && inspectStation && rssi;
}

bool NetworkControl::ScanAsync()
{
   auto scan = m_wpaControl.WpaCommand("SCAN");  
   return scan.outcome == Outcome::Success;
}

bool NetworkControl::CheckWpaStatus()
{
   if (m_stationStatusSubscriber == nullptr)
   {
      return false;
   }
   
   auto wpaStatus = m_wpaControl.WpaCommand("STATUS");
   auto lines = ToVectorString(wpaStatus.reply);
   auto wpa_state_text = GetLineContaining(lines, "wpa_state");
   
   if (wpa_state_text == "")
   {
      return false;
   }
   
   bool wpa_state = false;
   if (Contains(wpa_state_text, "COMPLETED"))
   {
      wpa_state = true;
   }
   
   if (wpa_state != m_wpaStationState.connectionStatus)
   {
      m_wpaStationState.connectionStatus = wpa_state;
      m_stationStatusSubscriber(wpa_state);
   }
   
   return true;
}

bool NetworkControl::HandleDhcpChangeAndNotify()
{
   DhcpTypes::DhcpStatusInfo dhcp;
   m_dhcp.GetDhcpStatus(m_wpaStationState.interfaceName, dhcp);

   if (m_wpaStationState.dhcp != dhcp)
   {
      m_wpaStationState.dhcp = dhcp;
   
      if (m_dhcpStatusSubscriber == nullptr)
      {
         return false;
      }
      m_dhcpStatusSubscriber(m_wpaStationState.dhcp);
   }
   
   return true;
}

VectorString  NetworkControl::GetStationConnections()
{
   VectorString connectedMacs;

   auto firstStation = m_wpaControl.HostApdCommand("STA-FIRST");
   if (firstStation.outcome != Outcome::Success)
   {
      return connectedMacs;
   }

   auto lines = ToVectorString(firstStation.reply);
   string currentMac;
   if (lines.size() >= 1)
   {
      currentMac = lines[0];
      connectedMacs.push_back(lines[0]);
   }

   while(currentMac.size())
   {
      string searchNextStation = "STA-NEXT " + currentMac;

      auto nextStation = m_wpaControl.HostApdCommand(searchNextStation);
      if (nextStation.outcome != Outcome::Success)
      {
         currentMac.clear();
         break;
      }
   
      lines = ToVectorString(nextStation.reply);
      if (lines.size() >= 1)
      {
         currentMac = lines[0];
         connectedMacs.push_back(currentMac);
      }
      else
      {
         currentMac.clear();
      }
   }

   return connectedMacs;
}

bool NetworkControl::CheckForNewStationConnections()
{
   auto currentlyConnectedMacs = GetStationConnections();                            
   auto newlyConnectedMacs = ExtractItemsNotInReferenceList(m_wpaStationState.connectedStations, currentlyConnectedMacs);
   auto newlyDisconnectedMacs = ExtractItemsNotInReferenceList(currentlyConnectedMacs, m_wpaStationState.connectedStations);
   
   m_wpaStationState.connectedStations = currentlyConnectedMacs;   

   if(m_stationConnectSubscriber != nullptr)
   {
      for (const auto &notificationMac : newlyConnectedMacs)
      {
         m_stationConnectSubscriber(true, notificationMac);
      }
      
      for (const auto &notificationMac : newlyDisconnectedMacs)
      {
         m_stationConnectSubscriber(false, notificationMac);
      }
      return true;
   }

   return false;
}

void NetworkControl::RequestRssiMeasurement()
{
   m_wpaStationState.rssiMeasurementRequired = true;
}

void NetworkControl::SetStationSubscriber(StationStatusSubscriber stationStatusSubscriber)
{
   m_stationStatusSubscriber = stationStatusSubscriber;
}

void NetworkControl::SetDhcpStatusSubscriber(DhcpStatusSubscriber dhcpStatusSubscriber)
{
   m_dhcpStatusSubscriber = dhcpStatusSubscriber;
}

void NetworkControl::SetStationConnectionSubscriber(StationConnectionSubscriber stationConnectionSubscriber)
{
   m_stationConnectSubscriber = stationConnectionSubscriber;
}

void NetworkControl::SetRssiMeasurementHandler(RssiMeasurementSubscriber rssiMeasurementSubscriber)
{
   m_rssiMeasurementSubscriber = rssiMeasurementSubscriber;
}
