#pragma once

#include "stdint_ex.h"
#include <string>

namespace DhcpTypes
{
    class DhcpStatusInfo
    {
      public:
         DhcpStatusInfo()
            : isBound(false)
            , leaseStartTime(0)
            , leaseTime(0)
         {
         }                                               

         bool operator==(const DhcpStatusInfo& rhs) const
         {
            return isBound == rhs.isBound &&
                   leaseStartTime == rhs.leaseStartTime &&
                   leaseTime == rhs.leaseTime &&
                   ipv4Address == rhs.ipv4Address &&
                   ipv4SubnetMask == rhs.ipv4SubnetMask &&
                   ipv4DefaultGateway == rhs.ipv4DefaultGateway &&
                   ipv4PrimaryDNS == rhs.ipv4PrimaryDNS &&
                   ipv4SecondaryDNS == rhs.ipv4SecondaryDNS;
         }

         bool operator!=(const DhcpStatusInfo& rhs) const
         {
            return !(*this == rhs);
         }
         
         bool isBound;     
         u32 leaseStartTime;
         u32 leaseTime;
         std::string ipv4Address;
         std::string ipv4SubnetMask;
         std::string ipv4DefaultGateway;
         std::string ipv4PrimaryDNS;
         std::string ipv4SecondaryDNS;
   };
}
