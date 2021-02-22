#pragma once

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

#define ___ KC_TRANSPARENT

#define LAYOUT(                                                                           \
    k00, k01, k02, k03, k04, k05,                        k0a, k0b, k0c, k0d, k0e, k0f,    \
    k10, k11, k12, k13, k14, k15,                        k1a, k1b, k1c, k1d, k1e, k1f,    \
    k20, k21, k22, k23, k24, k25,                        k2a, k2b, k2c, k2d, k2e, k2f,    \
    k30, k31, k32, k33, k34, k35, k36, k37,    k38, k39, k3a, k3b, k3c, k3d, k3e, k3f     \
    )                                                                                     \
    {                                               \
        /* Left Hand */                             \
        {k00, k01, k02, k03, k04, k05, ___, ___},  \
        {k10, k11, k12, k13, k14, k15, ___, ___},  \
        {k20, k21, k22, k23, k24, k25, k37, ___},  \
        {k30, k31, k32, k33, k34, k35, k36, ___},  \
                                                    \
        /* Right Hand */         \
        {k0f, k0e, k0d, k0c, k0b, k0a, ___, ___},  \
        {k1f, k1e, k1d, k1c, k1b, k1a, ___, ___},  \
        {k2f, k2e, k2d, k2c, k2b, k2a, k38, ___},  \
        {k3f, k3e, k3d, k3c, k3b, k3a, k39, ___}  \
    }
