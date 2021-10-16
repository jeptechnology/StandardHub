#pragma once

#include "LinuxDhcpInterface.h"

class LinuxDhcp : public LinuxDhcpInterface
{
public:
   LinuxDhcp();
   virtual VectorString GetDhcpcdDumpForInterface(std::string interface) override;
};

