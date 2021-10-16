#include "UtcTime.h"
#include <chrono>
#include <time.h>
u32 UtcTime::GetUtcTime()
{
   time_t now = time( NULL );
   return static_cast<u32>(now);
}