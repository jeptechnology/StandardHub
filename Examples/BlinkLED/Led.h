#pragma once

class Led
{
   int m_fd;

   int m_brightness1; // do this
   int m_delay1Ms;    // then fade into brightness2 for this many ms...
   int m_brightness2; // ...to this
   int m_delay2Ms;    // then fade back to brightness1 for this many ms...
   
public:
   Led(const char *ledName, const char *devicePath);

   void ToggleLed(void);
   void TurnLedOn(void);
   void TurnLedOff(void);

   void SetPattern(int m_brightness1,
                   int m_delay1Ms,
                   int m_brightness2,
                   int m_delay2Ms);
};