#ifndef YD_GENERAL_H
#define YD_GENERAL_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL2_gfxPrimitives.h"

// retrieves time in milliseconds; used for comparisons with another number retrieved this way,
// not for timing by real time!
#ifdef _WIN32
inline unsigned int getMSTime() { return GetTickCount(); }
#else
inline uint32 getMSTime()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

// retrieves time difference in milliseconds
inline unsigned int getMSTimeDiff(unsigned int oldMSTime, unsigned int newMSTime)
{
    // getMSTime() have limited data range and this is case when it overflow in this tick
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

#endif
