#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <thread>

class Button
{
   int m_fd;
   bool m_lastState;
   std::unique_ptr<std::thread> m_pollThread;   
   std::function<void(bool)> m_callback;
   
   void PollingTask();
   void FireEvent(bool newState);   

 public:
   Button(const char *devicePath);
   ~Button();
   
   bool IsDepressed();
   void RegisterHandler(std::function<void(bool)>&& handler);   
};

