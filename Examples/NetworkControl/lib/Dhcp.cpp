#include "Dhcp.h"
#include "Toolbox.h"
#include <cstring>
#include <iterator>
#include <algorithm>

using namespace Toolbox;
using namespace std;

namespace 
{
   constexpr char splitSeparator = '=';

   string RemoveQuotes(const string &line)
   {
      string sanitised;
      for (auto c : line)
      {
         if (c != '\'')
         {
               sanitised.push_back(c);
         }
      }
      return sanitised;
   }

   bool FindDhcpIntegerParameter(const VectorString dhcpLines, const char* parameter, u32& foundValue)
   {
      for (const auto &line : dhcpLines)
      {
         if (Contains(line, parameter))
         {
            auto sanitisedLine = RemoveQuotes(line);
            VectorString lineParts = ToVectorString(sanitisedLine, splitSeparator);
            if (lineParts.size() == 2)
            {
               auto convertedValue = ToInteger(lineParts[1]);
               foundValue = convertedValue;
               return true;
            }
         }
      }
      return false;
   }
   inline bool IsDigit(char test) 
   {
      if (test >= '0' && test <= '9')
      {
         return true;
      }
      else
      {
         return false;
      }
   }

   string ReplaceNonDigitsWithSpace(const string& input) 
   {
      std::string digitsOnly;

       for (const auto &singleChar : input)
       {
          if (IsDigit(singleChar))
          {
             digitsOnly.append(1,singleChar);
          }
          else
          {
             digitsOnly.append(1,' ');
          }
       }
      return digitsOnly;
   }

   bool ExtractFirstIPAddress(const string &workingString, string &foundvalue)
   {
      auto spacedNumbers = ReplaceNonDigitsWithSpace(workingString);
      auto numbers = ToVectorString(spacedNumbers, ' ' );
      if (numbers.size() >= 4)
      {

         u32 ipV4_0 = ToUnsignedInteger(numbers[0].c_str());
         u32 ipV4_1 = ToUnsignedInteger(numbers[1].c_str());
         u32 ipV4_2 = ToUnsignedInteger(numbers[2].c_str());
         u32 ipV4_3 = ToUnsignedInteger(numbers[3].c_str());
         
         const size_t ipStringMaximumLength = 16;
         char ipAddress[ipStringMaximumLength];
         snprintf(ipAddress, ipStringMaximumLength, "%d.%d.%d.%d", ipV4_0, ipV4_1, ipV4_2, ipV4_3);
         foundvalue = string(ipAddress);
         return true;
      }
      return false;
   }

   bool FindDhcpStringParameter(const VectorString dhcpLines, const char *parameter, string  &foundValue)
   {
      for (const auto &line : dhcpLines)
      {
         if (Contains(line, parameter))
         {
            auto sanitisedLine = RemoveQuotes(line);
            VectorString lineParts = ToVectorString(sanitisedLine, splitSeparator);
            if (lineParts.size() >= 2)
            {
               foundValue = lineParts[1];
               return true;
            }
         }
      }
      return false;
   }
}

Dhcp::Dhcp(LinuxDhcpInterface &linuxDhcpInterface, UtcTimeInterface &utc)
   : m_linuxDhcpInterface(linuxDhcpInterface)
   , m_utc(utc)
   , m_lastValidDhcpTime(0)
{
}

bool Dhcp::GetDNSServers(const string &interface, DhcpTypes::DhcpStatusInfo &dhcpInfo)
{
   auto interfaceDhcp = m_linuxDhcpInterface.GetDhcpcdDumpForInterface(interface);
   string dnsServerParameter;
   bool foundPrimaryDns = FindDhcpStringParameter(interfaceDhcp, "domain_name_servers", dnsServerParameter);

   if (foundPrimaryDns == false)
   {
      return false;
   }

   auto dnsSet = false;
   auto dnsSevers = ToVectorString(dnsServerParameter, ' ');
   
   if(dnsSevers.size() >= 1)
   {
      dnsSet = true;
      dhcpInfo.ipv4PrimaryDNS = dnsSevers[0];
   }

   if(dnsSevers.size() >= 2)
   {
      dhcpInfo.ipv4SecondaryDNS = dnsSevers[1];
   }
   return dnsSet;
}

bool Dhcp::GetRouter(const std::string &interface, DhcpTypes::DhcpStatusInfo &dhcpInfo)
{
   auto interfaceDhcp = m_linuxDhcpInterface.GetDhcpcdDumpForInterface(interface);
   string routers;
   bool foundDefaultGateway = FindDhcpStringParameter(interfaceDhcp, "routers", routers);

   if (foundDefaultGateway == false)
   {
      return false;
   }
   
   return ExtractFirstIPAddress(routers, dhcpInfo.ipv4DefaultGateway);
}

u32 Dhcp::TimeSinceLastValidDhcp()
{
   auto timeDelta =  m_utc.GetUtcTime() - m_lastValidDhcpTime;
   return timeDelta;
}

bool Dhcp::GetDhcpStatus(const string &interface, DhcpTypes::DhcpStatusInfo &dhcpInfo)
{
   auto interfaceDhcp = m_linuxDhcpInterface.GetDhcpcdDumpForInterface(interface);

   
   DhcpTypes::DhcpStatusInfo currentDhcp;

   bool foundIpv4Address = FindDhcpStringParameter(interfaceDhcp, "ip_address", currentDhcp.ipv4Address);
   FindDhcpIntegerParameter(interfaceDhcp, "dhcp_lease_time", currentDhcp.leaseTime);
   dhcpInfo.leaseStartTime = 0; // requires inspection of lease file timestamp
   FindDhcpStringParameter(interfaceDhcp, "subnet_mask", currentDhcp.ipv4SubnetMask);
   GetDNSServers(interface, currentDhcp);
   GetRouter(interface, currentDhcp);
   
   if (foundIpv4Address)
   {
      currentDhcp.isBound = true;
      m_cachedDhcp = currentDhcp;
      dhcpInfo = currentDhcp;
      m_lastValidDhcpTime = m_utc.GetUtcTime();
      return true;
   }
   else if (TimeSinceLastValidDhcp() < DhcpTimeoutSeconds)
   {
      dhcpInfo.isBound = true;
      dhcpInfo = m_cachedDhcp;
      return true;
   }
   
   m_cachedDhcp = currentDhcp;
   dhcpInfo = currentDhcp;
   return false;
}
