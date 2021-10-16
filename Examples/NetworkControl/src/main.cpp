#include<stdio.h>
#include <iostream>
#include "TestRunner.h"
#include "WpaControl.h"
#include <sys/timerfd.h>

using namespace std;

int main(int argc, char *argv[])
{
   bool run = true;
   
   while (run)
   {
      TestRunner::PrintTestOptions();
      auto input = TestRunner::GetInput();
      TestRunner::CallTestSequence(input);

      if (input == "x")
      {
         run = false;
      }
   }   
   
   cout << "End of example App";
}
