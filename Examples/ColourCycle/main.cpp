#include <iostream>
#include <unistd.h>

#include "Button.h"
#include "Led.h"

using namespace std;

int main(int argc, char *argv[])
{
   Button setup("setup", "/hardware/setupbutton/event1");
   
   Led    red  ("red",   "/hardware/leds/status_red/pattern");
   Led    green("green", "/hardware/leds/status_green/pattern");
   Led    blue ("blue",  "/hardware/leds/status_blue/pattern");

	cout << "Press setup button to switch led colour." << endl;

   Led* currentLed = &red;

   // register a callback for when the button changes state
   setup.RegisterHandler([&] (bool isPressed) {

      if (isPressed) return;
      
      currentLed->TurnLedOff();

      if (currentLed == &red)        { currentLed = &blue;  }
      else if (currentLed == &blue)  { currentLed = &green; }
      else if (currentLed == &green) { currentLed = &red;   }

      currentLed->TurnLedOn();
   });
   
   while (1)
   {
      sleep(1);
   }

	return 0;
}