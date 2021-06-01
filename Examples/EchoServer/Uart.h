#pragma once

#include <termios.h>
#include <chrono>

class Uart
{
   std::string m_deviceName;
   int m_fd;
   struct termios m_tty;

public:
   Uart(const char *deviceName);

   void PutChar(uint8_t byteToTransmit);
   void PutString(const uint8_t* stringToTransmit, u16 numberOfBytes);
   void PutString(const char* stringToTransmit);
   
   bool GetByteNonBlocking(uint8_t &byteToBeReceived);
   bool GetByte_BlockingFor(uint8_t &byteToBeReceived, std::chrono::milliseconds milliSecondsToBlock);
   u8   GetCharAndWaitForever();
   bool IsConnected() const;
   
   int GetFd() const { return m_fd; }
};
