#pragma once
#include "stdint_ex.h"

namespace WifiTypes
{
    static constexpr u8 ALL_CHANNELS = (u8)-1;
    static constexpr u8 ANY_CHANNEL = (u8)-1;
    static constexpr size_t MaximumSSIDLength = 32;
      
   enum AuthMode
   {
      AUTH_OPEN = 0,        /**< authenticate mode : open */
      AUTH_WEP,             /**< authenticate mode : WEP */
      AUTH_WPA_PSK,         /**< authenticate mode : WPA_PSK */
      AUTH_WPA2_PSK,        /**< authenticate mode : WPA2_PSK */
      AUTH_WPA_WPA2_PSK,    /**< authenticate mode : WPA_WPA2_PSK */
      AUTH_WPA2_ENTERPRISE, /**< authenticate mode : WPA2_ENTERPRISE */
      AUTH_MAX
   };

   struct AccessPointInfo
   {
      char ssid[MaximumSSIDLength+1];
      AuthMode auth;
      u8 channel;
      s8 rssi;
   };  
}
