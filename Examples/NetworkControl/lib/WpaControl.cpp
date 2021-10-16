#include "WpaControl.h"
#include <iostream>
#include <algorithm>

using namespace std;

extern "C"
{
   #include "wpa_ctrl.h"
}

class WpaControlLocker
{
 private:
   WpaControl &m_wpaControl;

 public:
   WpaControlLocker(WpaControl &wpaControl) 
      : m_wpaControl(wpaControl) 
   { 
      m_wpaControl.Lock(); 
   }
   ~WpaControlLocker() 
   { 
      m_wpaControl.Unlock(); 
   }
};

WpaControl::WpaControl() :
   	 m_lock(nullptr)
   , m_unlock(nullptr)
{
}

void WpaControl::SetLockUnlock(LockingFunction lock, LockingFunction unlock)
{
   m_lock = lock;
   m_unlock = unlock;
}

void WpaControl::CommandToUpperCase(string &str)
{
   for (auto &stringCharacter : str)
   {
      if (stringCharacter == ' ')
      {
         break;
      }

      stringCharacter = toupper(stringCharacter);
   }
}

Outcome WpaControl::ControlRequest(struct wpa_ctrl *controlConnection, const string &command, string &reply)
{
   Outcome outcome = Outcome::Fail;
   bool returnValue = false;

   char buffer[4096];
   size_t length = 4095;

   reply = "";
   int ret = wpa_ctrl_request(controlConnection, command.c_str(), command.length(), buffer, &length, NULL);

   if (ret == -2)
   {
      outcome =  Outcome::Timeout;
   }
   else if (ret < 0)
   {
      outcome = Outcome::Fail;
   }
   else if (ret == 0)
   {
      reply = string(buffer, length);
      outcome = Outcome::Success;
   }
   return outcome;
}

WpaControlResponse WpaControl::Transaction(string controlPath, string command)
{
   WpaControlLocker locker(*this);
   WpaControlResponse response;
   
   auto controlConnection = wpa_ctrl_open(controlPath.c_str());

   if (controlConnection == nullptr)
   {
      response.outcome = Outcome::UnableToConnect;
   
      return response;
   }

   response.outcome = ControlRequest(controlConnection, command, response.reply);
   wpa_ctrl_close(controlConnection);

   return response;
}

WpaControlResponse WpaControl::WpaCommand(string command)
{
   CommandToUpperCase(command);
   return Transaction(wpaSupplicantControlPath, command);
}

WpaControlResponse WpaControl::HostApdCommand(string command)
{
   CommandToUpperCase(command);
   return Transaction(hostApdControlPath, command);
}

void WpaControl::Lock()
{
   if (m_lock)
   {
      m_lock();
   }
}

void WpaControl::Unlock()
{
   if (m_unlock)
   {
      m_unlock();
   }
}
