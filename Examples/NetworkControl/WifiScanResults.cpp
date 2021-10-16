#include "WifiScanResults.h"
#include "Toolbox.h"
#include <array>

using namespace std;
using namespace Toolbox;

namespace
{
   u8 FrequencyToChannel(string &frequencyToConvert)
   {
      static const array<const char*,14 > frequencyToChannel = { "2412",
                                                                 "2417",
                                                                 "2422",
                                                                 "2427",
                                                                 "2432",
                                                                 "2437",
                                                                 "2442",
                                                                 "2447",
                                                                 "2452",
                                                                 "2457",
                                                                 "2462",
                                                                 "2467",
                                                                 "2472",
                                                                 "2484"  };

      u8 channel = 0;
      for (const auto &frequency : frequencyToChannel)
      {
         channel++;
         if (frequencyToConvert == frequency)
         {
            return channel;
         }
      }

      return 0;
   }

   void CopySSID(char *destination, string source)
   {
      size_t maxLength = source.size() < WifiTypes::MaximumSSIDLength ? source.size() : WifiTypes::MaximumSSIDLength;
      memset(destination, '\0',  WifiTypes::MaximumSSIDLength + 1);
      memcpy(destination, source.c_str(), maxLength);
   }

   bool Contains(const string &text, const char *searchString)
   {
      if (text.find(searchString) == string::npos)
      {
         return false;
      }
      else
      {
         return true;
      }
   }

   WifiTypes::AuthMode ToWifiAuthMode(const string &wifiFlags)
   {
      auto notPresent = string::npos;

      if (Contains(wifiFlags,"WPA2-EAP-CCMP"))
      {
         return WifiTypes::AUTH_WPA2_ENTERPRISE;
      }

      if (Contains(wifiFlags,"WPA-PSK-CCMP") && Contains(wifiFlags, "WPA2-PSK-CCMP"))
      {
         return WifiTypes::AUTH_WPA_WPA2_PSK;
      }
   
      if (Contains(wifiFlags,"WPA2-PSK-CCMP"))
      {
         return WifiTypes::AUTH_WPA2_PSK;
      }

      if (Contains(wifiFlags,"WPA-PSK-CCMP"))
      {
         return WifiTypes::AUTH_WPA_PSK;
      }

      if (Contains(wifiFlags,"WEP")) // a fairly safe assumption but need to check with a real WEP wifi network
      {
         return WifiTypes::AUTH_WEP;
      }
      return WifiTypes::AUTH_OPEN;
   }
}

namespace WifiScanResults
{
   vector<WifiTypes::AccessPointInfo> ToScanResults(const VectorString &lines, u8 selectedChannel)
   {
      vector<WifiTypes::AccessPointInfo> scanResults;
      constexpr size_t ExpectedTableColumns = 5;
      
      for (const auto &line : lines)
      {
         auto tableColumn = ToVectorString(line, '\t');
         
         if (tableColumn.size() != ExpectedTableColumns)
         {
            continue;
         }


         WifiTypes::AccessPointInfo accessPointInfo;
         accessPointInfo.channel = FrequencyToChannel(tableColumn[1]);
         accessPointInfo.rssi = ToInteger(tableColumn[2],-100);
         accessPointInfo.auth = ToWifiAuthMode(tableColumn[3]);
         CopySSID(&accessPointInfo.ssid[0], tableColumn[4]);

         if ((selectedChannel == WifiTypes::ANY_CHANNEL) || (selectedChannel == accessPointInfo.channel))
         {
            scanResults.push_back(accessPointInfo);
         }
         
      }
      return scanResults;
   }
}
