#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#if !defined(NDEBUG)
    #define DEBUG_MODE 0
    #define RELEASE_MODE 1
    #define MESSAGE_DELAY 500000 //500 ms = 500.000 us
    #define MESSAGE_LENGTH 51
#else
    #define DEBUG_MODE 1
    #define RELEASE_MODE 0
    #define MESSAGE_DELAY 200000 //200 ms = 200.000 us
    #define MESSAGE_LENGTH 11
#endif

#endif