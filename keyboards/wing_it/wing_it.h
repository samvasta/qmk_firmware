#pragma once

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

#define LAYOUT(                 \
    k00, k01,         \
    k10, k11)         \
    {                           \
        {k00, k10},   \
        {k01, k11}    \
    }
