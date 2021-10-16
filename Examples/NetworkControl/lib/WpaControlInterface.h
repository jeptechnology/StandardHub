#pragma once

#include "WpaControlResponse.h"

class WpaControlInterface
{
 public:
   virtual WpaControlResponse WpaCommand(std::string command) = 0;
   virtual WpaControlResponse HostApdCommand(std::string command) = 0;
protected:
   WpaControlInterface() {};
};
