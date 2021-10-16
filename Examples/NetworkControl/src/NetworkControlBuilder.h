#pragma once

#include "WpaControl.h"
#include "NetworkControl.h"
#include "LinuxDhcp.h"
#include "Dhcp.h"
#include "UtcTime.h"

class NetworkControlBuilder
{
 public:
   NetworkControlBuilder();
   NetworkControl& GetNetworkControl();

   UtcTime &GetUtcTime();

 private:
   UtcTime utcTime;
   Dhcp dhcp;
   LinuxDhcp linuxDhcp;
   WpaControl wpaControl;
   NetworkControl networkControl;
};