#pragma once
#include "VectorString.h"

class LinuxDhcpInterface
{
public:
   virtual VectorString GetDhcpcdDumpForInterface(std::string interface) = 0;
protected:
   LinuxDhcpInterface() {};
};
