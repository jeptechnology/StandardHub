#pragma once

#include <iostream>
#include "WpaControlInterface.h"
#include "DhcpInterface.h"
#include "stdint_ex.h"
#include "WifiTypes.h"
#include "VectorString.h"
#include <vector>
#include <functional>

class NetworkControl
{
 public:
   static constexpr u8 ANY_CHANNEL = (u8)-1;
   typedef std::function<void (bool)> StationStatusSubscriber;
   typedef std::function<void (DhcpTypes::DhcpStatusInfo &)> DhcpStatusSubscriber;
   typedef std::function<void (bool isConnected, const std::string remoteMac)> StationConnectionSubscriber;
   typedef std::function<void (s8 rssiMeasurement)> RssiMeasurementSubscriber;   

   NetworkControl(WpaControlInterface &wpaControl, DhcpInterface &dhcp);

   bool Initialise();

   // Station Functions
   bool Connect(std::string ssid, std::string passphrase);
   bool Disconnect();
   bool Reconnect();
   std::vector<WifiTypes::AccessPointInfo> Scan(u8 channel = WifiTypes::ALL_CHANNELS);
   void RequestRssiMeasurement();
   bool ReconfigureStation();

   // Access Point Functions
   bool EnableOpenAccessPoint(const std::string &SSID, u8 channel);
   bool EnableSecureAccessPoint(const std::string &SSID, const std::string &password, u8 channel);
   bool DisableAccessPoint();
   WpaControlResponse HostapdRaw(std::string rawCommand);
   WpaControlResponse WpaSupplicantRaw(std::string rawCommand);
   
   // Subscribers
   void SetStationSubscriber(StationStatusSubscriber stationStatusSubscriber);
   void SetDhcpStatusSubscriber(DhcpStatusSubscriber dhcpStatusSubscriber);
   void SetStationConnectionSubscriber(StationConnectionSubscriber stationConnectionSubscriber);
   void SetRssiMeasurementHandler(RssiMeasurementSubscriber rssiMeasurementSubscriber);
   
   // Background Activity
   bool DoPeriodicActivity(bool accessPointScan = true);
 private:
   bool CheckWpaStatus();
   bool DisableAccessPointIfEnabled();
   bool GetAccessPointState(std::string &accessPointState);
   bool SetAccessPointSSID(std::string ssid);
   bool SetAccessPointChannel(u8 channel);
   bool SetAccessPointState(bool enabled);
   bool ScanAsync();
   bool HandleDhcpChangeAndNotify();
   bool CheckForNewStationConnections();
   VectorString GetStationConnections();
   bool CheckForRssiMeasurementRequest();
   bool SetAccessPointSecurity(std::string password);
   WpaControlInterface &m_wpaControl;
   DhcpInterface &m_dhcp;
   
   static const uint16_t CommandBufferLen = 256;

   StationStatusSubscriber m_stationStatusSubscriber;
   DhcpStatusSubscriber m_dhcpStatusSubscriber;
   StationConnectionSubscriber m_stationConnectSubscriber;
   RssiMeasurementSubscriber m_rssiMeasurementSubscriber;
   
   class WpaStationState
   {
    public:
      WpaStationState();

      bool connectionStatus;
      std::string interfaceName;
      DhcpTypes::DhcpStatusInfo dhcp;
      VectorString connectedStations;
      bool rssiMeasurementRequired;
   };
   
    WpaStationState m_wpaStationState;
};

