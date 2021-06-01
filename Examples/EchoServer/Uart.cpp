#include <cstring>
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <unistd.h>  // write(), read(), close()
#include <sys/time.h> 
#include "Uart.h"

using namespace std;

namespace
{
   int fdAvailable(int fd, int waitTimeMs)
   {
      struct timeval tv;
      tv.tv_sec = (waitTimeMs / 1000);
      tv.tv_usec = (waitTimeMs * 1000) % 1000000;

      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(fd, &fds);
      return select(fd + 1, &fds, NULL, NULL, &tv);
   }
}

static bool IsValidFd(int fd)
{
   return fd > 0;   
}

Uart::Uart(const char *deviceName)
   : m_deviceName(deviceName)
{
   memset(&m_tty, 0, sizeof(m_tty));

   m_tty.c_iflag = 0;
   m_tty.c_oflag = 0;
   m_tty.c_cflag = CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
   m_tty.c_lflag = 0;
   m_tty.c_cc[VMIN] = 1;
   m_tty.c_cc[VTIME] = 5;

   if ((m_fd = open(deviceName, O_RDWR | O_NONBLOCK)) < 0)
   {
      cerr << "Unable to open serial device " << deviceName << " : " << strerror(errno) << endl;
   }
   else
   {
      // Assume 115200 for now
      cfsetospeed(&m_tty, B115200);
      cfsetispeed(&m_tty, B115200);
      cfmakeraw(&m_tty);
      
      tcsetattr(m_fd, TCSANOW, &m_tty);
   }
}

void Uart::PutChar(u8 byteToTransmit) 
{
   PutString(&byteToTransmit, 1);
}

void Uart::PutString(const u8* stringToTransmit, u16 numberOfBytes) 
{
   if (IsConnected())
   {
      auto bytesWritten = write(m_fd, stringToTransmit, numberOfBytes);
      if (bytesWritten < stringToTransmit)
      {
         cerr << "Failed to write all bytes to UART" << endl;
      }
   }
}

bool Uart::GetByteNonBlocking(u8 &byteToBeReceived) 
{
   if (IsConnected() && read(m_fd, &byteToBeReceived, 1) > 0)
   {
      return true;
   }
   return false;
}

bool Uart::GetByte_BlockingFor(u8 &byteToBeReceived, std::chrono::milliseconds milliSecondsToBlock) 
{
   if (!IsConnected())
   {
      sleep(milliSecondsToBlock / 1000);
      return false;
   }
   
   if (fdAvailable(m_fd, milliSecondsToBlock.count())) 
   {
      if (read(m_fd, &byteToBeReceived, 1) > 0)
      {
         return true;
      }
   }

   return false;
}

u8 Uart::GetCharAndWaitForever() 
{
   u8 c;
   while (!GetByte_BlockingFor(c, std::chrono::seconds(1)));
   return c;
}


bool Uart::IsConnected() const 
{
   return m_fd >= 0;
}