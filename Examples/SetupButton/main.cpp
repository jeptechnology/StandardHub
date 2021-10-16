#include <iostream>
#include <unistd.h>

#include "Button.h"

using namespace std;

void ButtonCallback(bool isPressed)
{
   cout << "Setup button is now " << (isPressed ? "pressed" : "released") << endl;
}

int main(int argc, char *argv[])
{
   Button setup("/hardware/setupbutton/event1");

	cout << "Setup button test." << endl;

   // register a callback for when the button changes state
   setup.RegisterHandler(ButtonCallback);
   
   while (1)
   {
      sleep(1);
   }

	return 0;
}