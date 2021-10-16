#include "LinuxDhcp.h"
#include "Toolbox.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <array>


#include <fstream>
#include <iostream>

#include <sstream> //std::stringstream
using namespace std;
using namespace Toolbox;

namespace 
{
   string FileToString(const string &fileName)
   {
      ifstream inFile;
      inFile.open(fileName.c_str());

      stringstream strStream;
      strStream << inFile.rdbuf();       
      string fileContents = strStream.str(); 

      return fileContents;
   }

 }  
LinuxDhcp::LinuxDhcp()
{
}

VectorString LinuxDhcp::GetDhcpcdDumpForInterface(string interface)
{
   string path = "/run/dhcpcd_status/" + interface + ".lease";
   string fileContents = FileToString(path);
   auto dhcpcdLines = ToVectorString(fileContents);
   return dhcpcdLines;
}

