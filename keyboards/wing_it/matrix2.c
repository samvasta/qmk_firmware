
#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"

#include "wait.h"
#include "print.h"
#include "debug.h"
#include "timer.h"
#include "wing_it.h"


#define print_matrix_header()  print("\nr/c 01234567\n")
#define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#define matrix_bitpop(i)       bitpop(matrix[i])

/* matrix state(1:on, 0:off) */
static matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
static matrix_row_t matrix[MATRIX_ROWS];      // debounced values
static const uint8_t col_masks[MATRIX_COLS] = MATRIX_COL_MASKS;


static const uint8_t row_masks[MATRIX_ROWS] = MATRIX_ROW_MASKS;

// matrix code

static void select_row(uint8_t row);
static void select_col(uint8_t col);


inline
matrix_row_t matrix_get_row(uint8_t row) {
    /* Matrix mask lets you disable switches in the returned matrix data. For example, if you have a
       switch blocker installed and the switch is always pressed. */
#ifdef MATRIX_MASKED
    return matrix[row] & matrix_mask[row];
#else
    return matrix[row];
#endif
}

void matrix_print(void) {
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

static void init_pins(void) {
    setPinOutput(ROW_SELECT_A);
    setPinOutput(ROW_SELECT_B);
    setPinOutput(ROW_SELECT_C);

    setPinOutput(COL_SELECT_A);
    setPinOutput(COL_SELECT_B);
    setPinOutput(COL_SELECT_C);
    setPinInput(COL_VALUE);
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool matrix_changed = false;

    // Select col and wait for col selecton to stabilize
    select_col(current_col);

    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Store last value of row prior to reading
        matrix_row_t last_row_value = current_matrix[row_index];

        select_row(row_index);

        // Check row pin state
        if (readPin(COL_VALUE) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= (MATRIX_ROW_SHIFTER << current_col);
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << current_col);
        }

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[row_index]) && !(matrix_changed)) {
            matrix_changed = true;
        }
    }

    return matrix_changed;
}


void matrix_init(void) {

    // JTAG disable for PORT F. write JTD bit twice within four cycles.
    MCUCR |= (1<<JTD);
    MCUCR |= (1<<JTD);

    // initialize key pins
    init_pins();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        raw_matrix[i] = 0;
        matrix[i]     = 0;
    }

    debounce_init(MATRIX_ROWS);

    matrix_init_quantum();
}

uint8_t matrix_scan(void) {
    bool changed = false;

    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
        changed |= read_rows_on_col(raw_matrix, current_col);
    }

    debounce(raw_matrix, matrix, MATRIX_ROWS, changed);

    matrix_scan_quantum();

    wait_ms(1000);
    return (uint8_t)changed;
}



static void select_row(uint8_t row) {
    uint8_t mask = row_masks[row];

    // dprintf("Row Mask: %d\n", mask);
    if(mask & 0x1)
    {
        writePinHigh(ROW_SELECT_A);
    }
    else
    {
        writePinLow(ROW_SELECT_A);
    }

    if((mask & 0x2) >> 1)
    {
        writePinHigh(ROW_SELECT_B);
    }
    else
    {
        writePinLow(ROW_SELECT_B);
    }

    if((mask & 0x4) >> 2)
    {
        writePinHigh(ROW_SELECT_C);
    }
    else
    {
        writePinLow(ROW_SELECT_C);
    }

    wait_us(25);
}

static void select_col(uint8_t col) {
    uint8_t mask = col_masks[col];

    if(mask & 0x1)
    {
        writePinHigh(COL_SELECT_A);
    }
    else
    {
        writePinLow(COL_SELECT_A);
    }

    if((mask & 0x2) >> 1)
    {
        writePinHigh(COL_SELECT_B);
    }
    else
    {
        writePinLow(COL_SELECT_B);
    }

    if((mask & 0x4) >> 2)
    {
        writePinHigh(COL_SELECT_C);
    }
    else
    {
        writePinLow(COL_SELECT_C);
    }

    wait_us(25);
}
