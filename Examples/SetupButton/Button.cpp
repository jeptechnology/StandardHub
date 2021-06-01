#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "Button.h"

// Local private functions
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

Button::Button(const char *name, const char* devicePath)
   : m_fd(0)
   , m_lastState(false)
   , m_callback(nullptr)   
{
   if (devicePath)
   {
      m_fd = open(devicePath, O_RDONLY);
      if (m_fd > 0)
      {         
         m_pollThread = std::make_unique<std::thread>(0, Os::ThreadPriority::High, name, [this] { this->PollingTask(); });
      }
   }
}

Button::~Button()
{
   if (m_fd > 0)
   {
      close(m_fd);      
   }
}

void Button::PollingTask()
{
   struct input_event event;  
   
   while (true)
   {
      if (fdAvailable(m_fd, 1000) && read(m_fd, &event, sizeof(event))) 
      {
         if (event.type == EV_KEY)
         {         
            FireEvent(event.value == 1);
         }
      }
   }
}

bool Button::IsDepressed()
{
   return m_lastState; 
}
   
void Button::RegisterHandler(std::function<void(bool)>&& handler)
{
   m_callback = handler;
}

void Button::FireEvent(bool newState)
{
   if ((newState != m_lastState))
   {
      m_lastState = newState;
      
      if (m_callback)
      {
         m_callback(newState);         
      }
   }  
}

