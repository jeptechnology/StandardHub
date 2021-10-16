#pragma once

#include <string>
#include "Outcome.h"

struct WpaControlResponse
{
public:
   WpaControlResponse() : outcome(Outcome::Fail)
   {
   }

   Outcome outcome;
   std::string reply;
};
