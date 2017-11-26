#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#ifndef BYTE
#define BYTE uint8_t
#endif
#ifndef SIZE
#define SIZE int32_t
#endif

#ifndef BOOL
#define BOOL int
#endif
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef xalloc
#define xalloc malloc
#define xfree free
#endif

static double PI = 3.1415926535897932384626433832795;
#define TO_RADIANS(angle) ((angle/180)*PI)

#include "hexlist.h"
#include "hexgraph.h"




