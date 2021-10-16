#include <iostream>
#include "TestSequences.h"
#include "WpaControl.h"
#include "NetworkControl.h"
#include "LinuxDhcp.h"
#include "Dhcp.h"
#include "NetworkControlBuilder.h"
#include "Toolbox.h"
#include <chrono>
#include <thread>
#include "stdint_ex.h"

using namespace std;
using namespace Toolbox;

#define Stringify(x) #x

namespace
{
   string to_authentication_string(WifiTypes::AuthMode authenticationMode)
   {
      using namespace  WifiTypes;
      
      switch (authenticationMode)
      {
      case (AuthMode::AUTH_WEP):
         return Stringify(AuthMode::AUTH_WEP);
      case (AuthMode::AUTH_WPA2_ENTERPRISE):
         return Stringify(AuthMode::AUTH_WPA2_ENTERPRISE);
      case (AuthMode::AUTH_WPA2_PSK):
         return Stringify(AuthMode::AUTH_WPA2_PSK);
      case (AuthMode::AUTH_WPA_WPA2_PSK):
         return Stringify(AuthMode::AUTH_WPA_WPA2_PSK);         
      case (AuthMode::AUTH_OPEN):
         return Stringify(AuthMode::AUTH_OPEN);         
      default:
         break;
      }
      return "";
   }
      
   void SleepForMs(u32 sleepMs)
   {
      this_thread::sleep_for(chrono::milliseconds(sleepMs));
   }
   
   s32 PromptForValue(const string prompt, const s32 defaultValue)
   {
      cout <<  prompt << endl << endl;
      string command;
      getline(cin, command);
      
      s32 promptValue = defaultValue;
      try
      {
         promptValue = stoi(command);
      }
      catch (exception& e) 
      {
      }
      return promptValue;
   }

   void PrintDhcpInfo(DhcpTypes::DhcpStatusInfo &dhcp)
   {
      cout << "DHCP Information" << endl;
      cout << "Lease start UTC: " << dhcp.leaseStartTime << endl;
      cout << "Bound: " << dhcp.isBound << endl;
      cout << "Lease time: " << dhcp.leaseTime << endl;
      cout << "Lease start time: " << dhcp.leaseStartTime << endl;
      cout << "ipv4Address:" << dhcp.ipv4Address << endl;
      cout << "ipv4DefaultGateway:" << dhcp.ipv4DefaultGateway << endl;
      cout << "ipv4PrimaryDNS:" << dhcp.ipv4PrimaryDNS << endl;
      cout << "ipv4SecondaryDNS:" << dhcp.ipv4SecondaryDNS << endl;
      cout << "ipv4SubnetMask:" << dhcp.ipv4SubnetMask << endl;
      cout << endl;
   }
}

namespace TestSequences
{
   void ConnectToStation()
   {
      string ssid, passphrase; 
      cout <<  "Please type the SSID:" << endl << endl;
      cin >> ssid;
      
      cout <<  "Please type the passphrase:" << endl << endl;
      cin >> passphrase;
      
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      auto outcome = networkControl.Connect(ssid, passphrase);
      
      cout << "Connect returned: " << DescribeOutcome(outcome) <<  endl;
   }
 
   void ListNetworks()  
   {
      WpaControl wpaControl;
      auto networks = wpaControl.WpaCommand("list_networks");
      cout << "List network : " << endl;
      
      PrintAllLines(networks.reply);
   }
   
   void ListNetworkZero()
   {
      WpaControl wpaControl;
      auto ssidResponse = wpaControl.WpaCommand("get_network 0 ssid");
      
      auto pskResponse = wpaControl.WpaCommand("get_network 0 psk");
      
      cout << "List network 0 ssid: " << endl;
      PrintAllLines(ssidResponse.reply);
      
      cout << "List network 0 psk: " << endl;
      PrintAllLines(pskResponse.reply);
   }
      
   void Disconnect()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      cout << "Disconnecting..." << endl;
      
      auto disconnect = networkControl.Disconnect();
      cout << "Disconnect returned: " << DescribeOutcome(disconnect) <<  endl;
   }
   
   void Reconnect()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      cout << "Reconnecting..." << endl;
      
      auto reconnect = networkControl.Reconnect();
      cout << "Reconnect returned: " << DescribeOutcome(reconnect) <<  endl;
   }
   
   void EnableOpenAccessPoint()
   {
      string ssid, authenticationKey, channel; 
      cout <<  "Please type the SSID:" << endl << endl;
      cin >> ssid;
      
      cout <<  "Please type the channel:" << endl << endl;
      cin >> channel;
      
      auto channelNumeric = stoi(channel);
      
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      auto initialised = networkControl.Initialise();
      auto outcome = networkControl.EnableOpenAccessPoint(ssid, channelNumeric);
      
      cout << "Initialised:" << DescribeOutcome(initialised) << endl;
      cout << "EnableOpenAccessPoint:" << DescribeOutcome(outcome) <<  endl;
   }
   
   void DisableAccessPoint()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      auto initialised = networkControl.Initialise();
      auto disabledAccessPoint = networkControl.DisableAccessPoint();
      
      cout << "Initialised:" << DescribeOutcome(initialised) << endl;
      cout << "DisableAccessPoint:" << DescribeOutcome(disabledAccessPoint) <<  endl;
   }
   
   void HostApdRawCommands()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      networkControl.Initialise();
   
      while (true)
      {
         string command;
         cout <<  "Please issue a command, or 'x' to quit:" << endl << endl;
         getline(cin, command);
         
         if (command == "x")
         {
            cout << "Ending" << endl;
            break;
         }
         else
         {
            cout << "Sending: " << command << endl;
            auto response = networkControl.HostapdRaw(command);
            cout << "Returned: " << endl << response.reply << endl;
         }
      }
   }
   
   void WpaSupplicantRawCommands()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      networkControl.Initialise();
      
      while (true)
      {
         string command;
         cout <<  "Please issue a command, or 'x' to quit:" << endl << endl;
         getline(cin, command);
         
         if (command == "x")
         {
            cout << "Ending" << endl;
            break;
         }
         else
         {
            cout << "Sending: " << command << endl;
            auto response = networkControl.WpaSupplicantRaw(command);
            cout << "Returned: " << endl << response.reply << endl;
         }
      }
   }
   
   void ScanForNetworks()
   {
      auto value = PromptForValue("Please enter the channel number or default to all channels",WifiTypes::ALL_CHANNELS);
      auto channel = static_cast<u8>(value);
      
      cout << "Scanning for channel value (0xFF = ANY): " << int(channel) << endl;
      
      
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      networkControl.Initialise();
      
      cout << "Running periodic activity (scan) - resulted printed in 3 seconds" << endl << endl;
      
      networkControl.DoPeriodicActivity();  // normally called in a timer task in the calling application
      
    
      SleepForMs(3000);
      
      auto scanResults = networkControl.Scan(channel);
      
      for (auto scanResult : scanResults)
      {
         cout << "Network: " << endl;
         cout << "SSID: " << scanResult.ssid << endl;
         cout << to_authentication_string(scanResult.auth) << endl;
         cout << "RSSI:" << to_string(scanResult.rssi) << endl;
         cout << "Channel:" << to_string(scanResult.channel) << endl;
         cout << endl;         
      }
   }
   
   void WpaStatus()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      networkControl.Initialise();
      
      bool currentConnectionStatus;
      auto OnStationConnectionChanged = [&currentConnectionStatus] (bool stationConnectionStatus)
                                        {
                                          currentConnectionStatus = stationConnectionStatus;
                                        };
      
      networkControl.SetStationSubscriber(OnStationConnectionChanged);
      networkControl.DoPeriodicActivity();
      
      if (currentConnectionStatus)
      {
         cout << "Connected to a station... will disconnect for a few seconds, and then reconnect to test notification...." << endl;
         
         cout << "Disconnecting..." << endl;
         networkControl.Disconnect();
         SleepForMs(2000);
         networkControl.DoPeriodicActivity();
         cout << "In client code context current connection status is: " << currentConnectionStatus << endl;
         
         SleepForMs(2000);
         
         cout << "Reconnectinng..." << endl;
         networkControl.Reconnect();
         SleepForMs(2000);
         networkControl.DoPeriodicActivity();
         cout << "In client code context current connection status is: " << currentConnectionStatus << endl;
      }
   }
   
   void GetDHCPInformation()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      DhcpTypes::DhcpStatusInfo currentDhcp;
      auto dhcpStatusSubscriber = [&currentDhcp] (DhcpTypes::DhcpStatusInfo &notifiedDhcp)
      {
         currentDhcp = notifiedDhcp;
      };
      
      networkControl.SetDhcpStatusSubscriber(dhcpStatusSubscriber);
      networkControl.Initialise();
   
      PrintDhcpInfo (currentDhcp);
   }
 
   void MonitorStationConnections()
   {
      string ssid, authenticationKey, channel; 
      cout <<  "Please type the SSID:" << endl << endl;
      cin >> ssid;
      
      cout <<  "Please type the channel:" << endl << endl;
      cin >> channel;
      
      auto channelNumeric = stoi(channel);
      
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      
      auto macSubscriber = [](bool isConnected, const string mac)
      {
         cout << "Mac connected" << mac << endl;
         cout << "Is connected:" << isConnected << endl;
         
      };
      
      auto initialised = networkControl.Initialise();
      networkControl.SetStationConnectionSubscriber(macSubscriber);
      auto outcome = networkControl.EnableOpenAccessPoint(ssid, channelNumeric);
      
      cout << "Initialised:" << DescribeOutcome(initialised) << endl;
      cout << "EnableOpenAccessPoint:" << DescribeOutcome(outcome) <<  endl;
      cout << "Monitoring station connections, press 'x' to quit" << endl;
      
      while (true)
      {
         SleepForMs(1000);
         networkControl.DoPeriodicActivity();
         
         if (cin.rdbuf()->in_avail())
         {
            string command;
            getline(cin, command);
            if (command == "x")
            {
               cout << "Ending" << endl;
               break;
            }
         }
      }
   }
   
   void WifiRssiMeasurement()
   {
      const s8 DefaultNoMeasurement = -128;
      s8 actualRssi = DefaultNoMeasurement;     
      auto rssiSubscriber = [&actualRssi] (s8 rssiMeasurement)
      {
         actualRssi = rssiMeasurement;
      };
      
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      networkControl.SetRssiMeasurementHandler(rssiSubscriber);
      networkControl.Initialise();
      
      networkControl.RequestRssiMeasurement();
      
      networkControl.DoPeriodicActivity();
      
      if (actualRssi == DefaultNoMeasurement)
      {
         cout << "No RSSI measurement was taken. Check wlan0 is connected to wifi" << endl;
         return;
      }
      
      cout << "RSSI: " << static_cast<s32>(actualRssi) << endl;
   }

   void PrintDhcpInfoInLoop()
   {
      cout << "Printing wlan 0 DHCP info in infinite loop, press 'x' to quit" << endl;
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      UtcTime &time = builder.GetUtcTime();
      DhcpTypes::DhcpStatusInfo currentDhcp;
      auto dhcpStatusSubscriber = [&currentDhcp] (DhcpTypes::DhcpStatusInfo &notifiedDhcp)
      {
         currentDhcp = notifiedDhcp;
      };
      
      networkControl.SetDhcpStatusSubscriber(dhcpStatusSubscriber);
      networkControl.Initialise();
      
      while (true)
      {
         cout << "Time: " << time.GetUtcTime() << endl;
         networkControl.DoPeriodicActivity();
         PrintDhcpInfo (currentDhcp);

         if (cin.rdbuf()->in_avail())
         {
            string command;
            getline(cin, command);
            if (command == "x")
            {
               cout << "Ending" << endl;
               break;
            }
         }
         SleepForMs(5000);
      }
   }

   void ReconfigureStation()
   {
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      auto initialised = networkControl.Initialise();
      auto disabledAccessPoint = networkControl.ReconfigureStation();
      
      cout << "Initialised:" << DescribeOutcome(initialised) << endl;
      cout << "ReconfigureStation:" << DescribeOutcome(disabledAccessPoint) <<  endl;
   }

   void EnableSecureAccessPoint()
   {
       string ssid, password, authenticationKey, channel; 
      cout <<  "Please type the SSID:" << endl << endl;
      cin >> ssid;
      
      cout <<  "Please type the password:" << endl << endl;
      cin >> password;
      
      cout <<  "Please type the channel:" << endl << endl;
      cin >> channel;
      
      auto channelNumeric = stoi(channel);
      
      NetworkControlBuilder builder;
      NetworkControl &networkControl = builder.GetNetworkControl();
      
      auto initialised = networkControl.Initialise();
      auto outcome = networkControl.EnableSecureAccessPoint(ssid, password, channelNumeric);
      
      cout << "Initialised:" << DescribeOutcome(initialised) << endl;
      cout << "EnableOpenAccessPoint:" << DescribeOutcome(outcome) <<  endl;
   }
}

