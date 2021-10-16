#pragma once

namespace TestSequences
{
   void ConnectToStation();
   void ListNetworkZero();
   void ListNetworks();
   void Disconnect();
   void Reconnect();
   void ReconfigureStation();
   void EnableOpenAccessPoint();
   void EnableSecureAccessPoint();
   void DisableAccessPoint();
   void HostApdRawCommands();
   void WpaSupplicantRawCommands();
   void ScanForNetworks();
   void WpaStatus();
   void GetDHCPInformation();
   void MonitorStationConnections();
   void WifiRssiMeasurement();
   void PrintDhcpInfoInLoop();
   
}

