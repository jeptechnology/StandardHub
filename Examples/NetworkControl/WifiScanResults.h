#pragma once

#include "WifiTypes.h"
#include "VectorString.h"

namespace WifiScanResults
{
   std::vector<WifiTypes::AccessPointInfo> ToScanResults(const VectorString &lines, u8 selectedChannel);
};

