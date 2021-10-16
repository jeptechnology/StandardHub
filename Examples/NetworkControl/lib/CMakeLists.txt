cmake_minimum_required(VERSION 3.15)

project(NetworkControlLibrary
    DESCRIPTION "Network Control Service"
    LANGUAGES C CXX
)

# add_definitions(-DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UDP)
add_definitions(-DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX)

add_library(NetworkControlLibrary STATIC
   Dhcp.cpp
   Dhcp.h
   DhcpInterface.h
   DhcpTypes.h
   LinuxDhcp.cpp
   LinuxDhcp.h
   LinuxDhcpInterface.h
   NetworkControl.cpp
   NetworkControl.h
   Outcome.h
   stdint_ex.h
   Toolbox.cpp
   Toolbox.h
   VectorString.h
   WifiScanResults.cpp
   WifiScanResults.h
   WifiTypes.h
   WpaControl.cpp
   WpaControl.h
   WpaControlInterface.h
   WpaControlResponse.h
   Wpa/build_config.h
   Wpa/common.h
   Wpa/includes.h
   Wpa/os.h
   Wpa/os_unix.c
   Wpa/wpa_ctrl.c
   Wpa/wpa_ctrl.h
   Wpa/wpa_debug.h
   Wpa/wpabuf.h
   UtcTime.cpp
   UtcTime.h)

   target_include_directories(NetworkControlLibrary PRIVATE Wpa)