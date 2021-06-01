#include <iostream>
#include <unistd.h>
#include <chrono>

#include "Uart.h"


using namespace std;
using std::chrono_literals;

int main(int argc, char *argv[])
{
   Uart uart("/hardware/cliuart");

	cout << "UART echo server." << endl;

   int counter = 0;

   while (1)
   {
      uint8_t c;
      if (uart.GetByte_BlockingFor(c, 5000ms))
      {  
         // echo back to user
         uart.PutChar(c);

         // log when we've received more than 10 bytes...
         if (++counter >= 10)
         {
            cout << "UART1 has recieved another 10 bytes" << endl;
         }
      }
      else
      {
         cout << "UART1 has been idle for 5 seconds" << endl;
      }
   }

	return 0;
}