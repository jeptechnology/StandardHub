#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cstring>

#include "Uart.h"


using namespace std;
using namespace std::chrono_literals;

const char *WelcomeMessage = "Welcome to the Echo Server\r\n";

int main(int argc, char *argv[])
{
   // Note that '/hardware/cliuart' is mapped from /dev/ttymxc1 from the standard hub Linux OS
   Uart uart("/hardware/cliuart");

	cout << "UART2 echo server." << endl;

   int counter = 0;

   uart.PutString((uint8_t*)WelcomeMessage, strlen(WelcomeMessage));

   while (1)
   {
      uint8_t c;
      if (uart.GetByte_BlockingFor(c, 5000ms))
      {  
         // echo back to user
         uart.PutChar(c);
         
         // output on stdout
         cout << c;
         if (c == '\r') cout << '\n';
         cout.flush();
      }
      else
      {
         cout << endl << "UART2 has been idle for 5 seconds" << endl;
      }
   }

	return 0;
}