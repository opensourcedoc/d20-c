#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #define SEP "\r\n"
#else
    #define SEP "\n"
#endif

#endif  // PLATFORM_H