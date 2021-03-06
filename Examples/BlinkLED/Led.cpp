#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cinttypes>
#include "Led.h"

using namespace std;

Led::Led(const char* ledName, const char *devicePath)
   : m_fd(0)
{
   if (devicePath)
   {      
      m_fd = open(devicePath, O_WRONLY);
      if (m_fd < 0)
      {
         cerr << ledName << "LED Error: Unable to open " << devicePath << endl;
         return;
      }
   }
   
   SetPattern(0, 0, 0, 0);
}
  
void Led::SetPattern(uint8_t  brightness1,
                     uint16_t delay1Ms,
                     uint8_t  brightness2,
                     uint16_t delay2Ms)
{
   m_brightness1 = brightness1;
   m_brightness2 = brightness2;
   m_delay1Ms = delay1Ms;
   m_delay2Ms = delay2Ms;

   if (m_fd > 0)
   {
      char buf[32];
      
      auto len = snprintf(buf, sizeof(buf), "%" PRIu8 " %" PRIu16 " %" PRIu8 " %" PRIu16 "\n",
         m_brightness1,   // start at this brightness...
         m_delay1Ms,      // then fade for this duration...
         m_brightness2,   // ...and end up at this brightness
         m_delay2Ms       // then fade for this duration back to brightness 1
         );
      
      if (write(m_fd, buf, len) < 0)
      {
         cerr << "LED Error: Unable to set state" << endl;
      }      
   }
}

void Led::ToggleLed(void)
{
   // bitwise NOT the brightness
   SetPattern(~m_brightness1, m_delay1Ms, ~m_brightness2, m_delay2Ms);
}

void Led::TurnLedOn(void)
{
   SetPattern(255, 1000, 255, 1000);
}
   
void Led::TurnLedOff(void) 
{
   SetPattern(0, 1000, 0, 1000);
}      
