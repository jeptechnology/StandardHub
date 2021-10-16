#pragma once

#include "DhcpTypes.h"
#include "LinuxDhcpInterface.h"
#include "UtcTimeInterface.h"
#include "DhcpInterface.h"

class Dhcp : public DhcpInterface
{
public:
   Dhcp(LinuxDhcpInterface& linuxDhcpInterface, UtcTimeInterface& utc);
   bool GetDhcpStatus(const std::string& interface, DhcpTypes::DhcpStatusInfo& dhcpTypes) override;
private:
   static const u32 DhcpTimeoutSeconds{ 30 };
   bool GetDNSServers(const std::string &interface, DhcpTypes::DhcpStatusInfo &dhcpInfo);
   bool GetRouter(const std::string &interface, DhcpTypes::DhcpStatusInfo &dhcpInfo);
   u32 TimeSinceLastValidDhcp();
   LinuxDhcpInterface &m_linuxDhcpInterface;
   UtcTimeInterface &m_utc;
   u32 m_lastValidDhcpTime;
   DhcpTypes::DhcpStatusInfo m_cachedDhcp;
};
