#pragma once

#include "WpaControlInterface.h"
#include <functional>

class WpaControlLocker;
class WpaControl : public WpaControlInterface
{
 public:
   typedef std::function<void ()> LockingFunction;
   WpaControl();

   WpaControlResponse WpaCommand(std::string command);
   WpaControlResponse HostApdCommand(std::string command);
   
   void SetLockUnlock(LockingFunction lock, LockingFunction unlock);
   
 private:
   WpaControlResponse Transaction(std ::string controlPath, std::string command);
   Outcome ControlRequest(struct wpa_ctrl *controlConnection, const std::string &command, std::string &reply);
   
   void CommandToUpperCase(std::string &str);

   const std::string hostApdControlPath = "/var/run/hostapd/wlan1";
   const std::string wpaSupplicantControlPath = "/var/run/wpa_supplicant/wlan0";
   
   void Lock();
   void Unlock();
   LockingFunction m_lock;
   LockingFunction m_unlock;

   friend class WpaControlLocker;
};
