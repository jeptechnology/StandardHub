#include "TestRunner.h"
#include "TestSequences.h"
#include <iostream>
#include <string>
#include <functional>
#include <vector>

using namespace std;

namespace 
{
   struct TestOptionToFunction
   {
      string optionIdentifier;
      std::function<void()> testFunction;   
      string description;
   };

   vector<TestOptionToFunction> optionMappings =
      {
         { "1", TestSequences::ConnectToStation, "Connect to station on WLan0 by user input of SSID and Passphrase required to connect to a network" },
         { "2", TestSequences::ListNetworks,     "List all networks from low level WpaControl" },
         { "3", TestSequences::ListNetworkZero,  "Detail current settings for WPA network 0 configuration from low level WpaControl" },
         { "4", TestSequences::Disconnect,       "Disconnect from station"},
         { "5", TestSequences::Reconnect,        "Reconnect to station"},
         { "6", TestSequences::EnableOpenAccessPoint,  "Enable open access point with user defined SSID"},
         { "7", TestSequences::DisableAccessPoint,     "Disable access point"},
         { "8", TestSequences::ScanForNetworks,     "Wifi network scan"},
         { "9", TestSequences::WpaStatus,           "If connected to a network, disconnect and reconnect looking for state notification change"},
         { "10", TestSequences::GetDHCPInformation, "Get DHCP on wlan0"},
         { "11", TestSequences::MonitorStationConnections, "Enable open access point and monitor connected macs"},
         { "12", TestSequences::WifiRssiMeasurement, "Take a spot measurement of wlan0 RSSI for wlan0 connected to wifi"},
         { "13", TestSequences::PrintDhcpInfoInLoop, "Print dhcp info for wlan0 in infinite loop"},
         { "14", TestSequences::ReconfigureStation, "Cause wlan0 to be reconfigured from wpa_supplicant settings"},
         { "15", TestSequences::EnableSecureAccessPoint, "Setup Access point with password authentication"},
         { "100", TestSequences::HostApdRawCommands,   "Raw Hostapd command access over IPC socket"},
         { "101", TestSequences::WpaSupplicantRawCommands,   "Raw wpa_supplicant command access over IPC socket"},
      };
}
namespace TestRunner
{
   string GetInput()
   {
      cout <<  "Please type your option:" << endl << endl;
      string input; 
      getline(cin, input);
      return input;
   }
   
   void CallTestSequence(string userOption)
   {
      for (auto option : optionMappings)
      {
         if (option.optionIdentifier == userOption)
         {
            cout << "Running test: " << option.optionIdentifier << endl;
            option.testFunction();
            cout << "Done " << endl;
            return;
         }
      }
   }
   
   void PrintTestOptions()
   {
      cout << endl;
      cout << "Available test options are: " << endl;
      for (auto option : optionMappings)
      {
         cout << option.optionIdentifier << ":" << option.description << endl;
      }
      cout << endl;
   }
}
