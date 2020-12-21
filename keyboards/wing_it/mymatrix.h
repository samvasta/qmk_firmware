#pragma once

#include <stdint.h>
#include <stdbool.h>
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include "matrix.h"

matrix_row_t getRow(uint8_t row);
