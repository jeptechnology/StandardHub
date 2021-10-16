#include "NetworkControlBuilder.h"

NetworkControlBuilder::NetworkControlBuilder()
   : dhcp(linuxDhcp, utcTime)
   , networkControl(wpaControl, dhcp)
{
}

NetworkControl& NetworkControlBuilder::GetNetworkControl()
{
   return networkControl;
}

UtcTime& NetworkControlBuilder::GetUtcTime()
{
   return utcTime;
}
