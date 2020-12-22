#include <stdint.h>
#include <stdbool.h>
// #include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include <print.h>
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

#define LED B4

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

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
  // setPinOutput(D6);
  // setPinOutput(LED);
  unselect_rows();
  init_cols();

  for(uint8_t i = 0; i < NUM_MODULES; i++){
    module_init(&modules[i]);
    // if(i % 2 == 0){
    //   writePinHigh(LED);
    // }
    // else {
    //   writePinLow(LED);
    // }
  }
  if((&modules[0])->status){
    writePinHigh(LED);
  }
  else{
    writePinLow(LED);
  }
  // writePinHigh(LED);

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
  unselect_rows();
  init_cols();

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

// uint16_t counter = 0;

void pulseLed(uint8_t num_times)
{
  for(uint8_t i = 0; i < num_times; i++){
    writePinHigh(D6);
    wait_us(250000); //Wait 1/2 sec.
    writePinLow(D6);
    wait_us(250000); //Wait 1/2 sec.
  }
    wait_us(500000); //Wait 1/2 sec.
    print("scanning\n");
}

uint8_t matrix_scan(void)
{
  // writePinLow(LED);
  // pulseLed(2);
  // writePinHigh(LED);
  // wait_us(500000); //Wait 1/2 sec.

  // print("Scanning:\n");

  if((&modules[0])->status){
    writePinHigh(LED);
  }
  else{
    writePinLow(LED);
  }

  for(uint8_t i = 0; i < NUM_MODULES; i++){
    module_scan(&modules[i]);
    // if(i % 2 == 0){
    //   writePinHigh(LED);
    // }
    // else {
    //   writePinLow(LED);
    // }
  }

  // clear_keyboard();


  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    select_row(i);
    wait_us(30);  // without this wait read unstable value.
    matrix_row_t mask = debounce_mask(i);
    matrix_row_t cols = (read_cols(i) & mask) | (matrix[i] & ~mask);
    debounce_report(cols ^ matrix[i], i);
    matrix[i] = cols;

    unselect_rows();
  }

  matrix_scan_quantum();
  // matrix_print();

  // pulseLed(5);
  // wait_us(1000000); //Wait 1/2 sec.

  // ++counter;
  // if(counter & 0b10) {
  //   writePinHigh(LED);
  // }
  // else{
  //   writePinLow(LED);
  // }

  return 1;
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

void matrix_print(void)
{
  print("\nr/c 0123456789ABCDEF\n");
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row); print(": ");
    pbin_reverse16(matrix_get_row(row));
    print("\n");
  }
}

uint8_t matrix_key_count(void)
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    count += bitpop16(matrix[i]);
  }
  return count;
}

static void init_cols(void)
{
  for(uint8_t i = 0; i < NUM_MODULES; i++){
    module_init(&modules[i]);
  }
}

static matrix_row_t read_cols(uint8_t row)
{
  matrix_row_t value = module_read_cols(&modules[0]);//MODULE_PTR_FROM_ROW(row));
  // uprintf("final row value = %d\n", value);
  return value;
}

static void unselect_rows(void)
{
  for(uint8_t i = 0; i < NUM_MODULES; i++){
    module_unselect_rows(&modules[i]);
  }
}

static void select_row(uint8_t row)
{
  // uprintf("Select row %d::", row);
  module_select_row(&modules[0], row);
}

