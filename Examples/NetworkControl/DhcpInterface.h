#pragma once

#include "DhcpTypes.h"

class DhcpInterface
{
public:
   virtual bool GetDhcpStatus(const std::string &interface, DhcpTypes::DhcpStatusInfo &dhcpTypes) = 0;
protected:
   DhcpInterface() {};
};

