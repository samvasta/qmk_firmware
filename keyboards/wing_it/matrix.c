#include <stdint.h>
#include <stdbool.h>
// #include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "module.h"
#include "quantum.h"
/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On Ergodox matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 317 scans/second, or about 3.15 msec/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * And so, there is no sense to have DEBOUNCE higher than 2.
 */

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif


#ifndef MODULE_PTR_FROM_ROW
#   define MODULE_PTR_FROM_ROW(row) &modules[row_to_module_map[row]]
#endif

#define LED D7

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

// static matrix_row_t read_cols(uint8_t row);
static void unselect_rows(void);
// static void select_row(uint8_t row);
static void scan_modules(void);

__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

inline
uint8_t matrix_rows(void)
{
  return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
  return MATRIX_COLS;
}

void matrix_init(void)
{
  scan_modules();
  unselect_rows();

  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    for (uint8_t j=0; j < MATRIX_COLS; ++j) {
      debounce_matrix[i * MATRIX_COLS + j] = 0;
    }
  }


  matrix_init_quantum();
}

void matrix_power_up(void) {
  scan_modules();
  unselect_rows();

  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
  }
}

// Returns a matrix_row_t whose bits are set if the corresponding key should be
// eligible to change in this scan.
matrix_row_t debounce_mask(uint8_t row) {
  matrix_row_t result = 0;
  for (uint8_t j=0; j < MATRIX_COLS; ++j) {
    if (debounce_matrix[row * MATRIX_COLS + j]) {
      --debounce_matrix[row * MATRIX_COLS + j];
    } else {
      result |= (1 << j);
    }
  }
  return result;
}

// Report changed keys in the given row.  Resets the debounce countdowns
// corresponding to each set bit in 'change' to DEBOUNCE.
void debounce_report(matrix_row_t change, uint8_t row) {
  for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
    if (change & (1 << i)) {
      debounce_matrix[row * MATRIX_COLS + i] = DEBOUNCE;
    }
  }
}

uint8_t matrix_scan(void)
{
  uint8_t row = 0;
  uint8_t changed = 0;

  for(uint8_t module_idx = 0; module_idx < NUM_MODULES; ++module_idx)
  {
    Module *module = MODULE_PTR_FROM_ROW(row);
    if(!module->status)
    {
      module_scan(module);
      wait_us(30);
    }

    for(uint8_t module_row = 0; module_row < module->num_rows; ++module_row)
    {
      module_select_row(module, module_row);
      wait_us(30);
      matrix_row_t mask = debounce_mask(row);

      //Turn off keys that can change
      matrix_row_t cols = matrix[row] & ~mask;

      //Read current matrix state
      matrix_row_t read = module_read_cols(module);
      // wait_us(30);

      //Turn on keys that can change and are pressed
      cols |= (~read & mask);
      uint8_t row_change = cols ^ matrix[row];
      changed |= row_change;
      debounce_report(row_change, row);
      matrix[row] = cols;

      ++row;
    }
  }

  matrix_scan_quantum();

  return changed;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
  return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
  return matrix[row];
}


static void unselect_rows(void)
{
  for(uint8_t i = 0; i < NUM_MODULES; i++){
    module_unselect_rows(&modules[i]);
  }
}

static void scan_modules()
{
  for(uint8_t i = 0; i < NUM_MODULES; i++)
  {
    module_scan(MODULE_PTR_FROM_ROW(i));
  }
}
