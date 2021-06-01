#include <iostream>
#include <unistd.h>

#include "led.h"

using namespace std;

int main(int argc, char *argv[])
{
   Led red  ("red",   "/hardware/leds/status_red/pattern");
   Led green("green", "/hardware/leds/status_green/pattern");
   Led blue ("blue",  "/hardware/leds/status_blue/pattern");

	cout << "LED test." << endl;

   while (1)
   {      
   	cout << "RED" << endl;
      red.TurnLedOn();
      sleep(5);     
      red.TurnLedOff();

   	cout << "BLUE" << endl;
      blue.TurnLedOn();
      sleep(5); 
      blue.ToggleLed(); // toggle here will turn it off

   	cout << "GREEN fade" << endl;
      green.SetPattern(255, 500, 0, 500); // fade between full (255) and off (0) half a second between each fade pattern
      sleep(10);
      green.TurnLedOff();
   }

	return 0;
}