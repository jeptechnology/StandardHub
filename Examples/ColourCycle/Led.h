#pragma once

#include <cstdint>

class Led
{
   int m_fd;

   uint8_t  m_brightness1; // do this
   uint16_t m_delay1Ms;    // then fade into brightness2 for this many ms...
   uint8_t  m_brightness2; // ...to this
   uint16_t m_delay2Ms;    // then fade back to brightness1 for this many ms...
   
public:
   Led(const char *ledName, const char *devicePath);

   void ToggleLed(void);
   void TurnLedOn(void);
   void TurnLedOff(void);

   void SetPattern(uint8_t  brightness1,
                   uint16_t delay1Ms,
                   uint8_t  brightness2,
                   uint16_t delay2Ms);
};