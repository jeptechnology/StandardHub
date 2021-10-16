#pragma once
#include  <string>

namespace TestRunner
{
   std::string GetInput();
   void PrintTestOptions();
   void CallTestSequence(std::string userOption);
}
