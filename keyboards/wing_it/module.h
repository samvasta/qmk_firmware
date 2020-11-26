#ifndef MODULE_H
#define MODULE_H

#include "quantum.h"
#include <stdint.h>
#include "config.h"
#include "matrix.h"
#include "i2c_master.h"
#include <util/delay.h>

#define MCP23017_A0 0
#define MCP23017_A1 0
#define MCP23017_A2 0

#define I2C_TIMEOUT 100

#define EXPANDER_ADDR(a0, a1, a2) ((0x20|(a0<<0)|(a1<<1)|(a2<<2)) << 1)
typedef enum EXPANDER_REG_BANK0 {
  EXPANDER_REG_IODIRA = 0,
  EXPANDER_REG_IODIRB,
  EXPANDER_REG_IPOLA,
  EXPANDER_REG_IPOLB,
  EXPANDER_REG_GPINTENA,
  EXPANDER_REG_GPINTENB,
  EXPANDER_REG_DEFVALA,
  EXPANDER_REG_DEFVALB,
  EXPANDER_REG_INTCONA,
  EXPANDER_REG_INTCONB,
  EXPANDER_REG_IOCONA,
  EXPANDER_REG_IOCONB,
  EXPANDER_REG_GPPUA,
  EXPANDER_REG_GPPUB,
  EXPANDER_REG_INTFA,
  EXPANDER_REG_INTFB,
  EXPANDER_REG_INTCAPA,
  EXPANDER_REG_INTCAPB,
  EXPANDER_REG_GPIOA,
  EXPANDER_REG_GPIOB,
  EXPANDER_REG_OLATA,
  EXPANDER_REG_OLATB
} expander_reg;

// typedef expander_reg EXPANDER_REG_BANK0;

typedef struct expander_pin {
  expander_reg reg;
  uint8_t pin;
} expander_pin;

#define EXPANDER_A8 { .pin=0b10000000, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A7 { .pin=0b01000000, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A6 { .pin=0b00100000, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A5 { .pin=0b00010000, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A4 { .pin=0b00001000, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A3 { .pin=0b00000100, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A2 { .pin=0b00000010, .reg=EXPANDER_REG_GPIOA }
#define EXPANDER_A1 { .pin=0b00000001, .reg=EXPANDER_REG_GPIOA }

#define EXPANDER_B8 { .pin=0b10000000, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B7 { .pin=0b01000000, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B6 { .pin=0b00100000, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B5 { .pin=0b00010000, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B4 { .pin=0b00001000, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B3 { .pin=0b00000100, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B2 { .pin=0b00000010, .reg=EXPANDER_REG_GPIOB }
#define EXPANDER_B1 { .pin=0b00000001, .reg=EXPANDER_REG_GPIOB }

// const expander_pin A8 = (expander_pin){ .pin=0b10000000, .reg=EXPANDER_REG_GPIOA };

typedef struct Module {
  uint8_t status;
  uint8_t address;
  uint8_t num_rows;
  uint8_t num_cols;
  expander_pin col_map[MAX_MODULE_COLS];
  expander_pin row_map[MAX_MODULE_ROWS];
} Module;


void module_init(Module *module);

void module_scan(Module *Module);

matrix_row_t module_read_cols(Module *module);

void module_unselect_rows(Module *module);

void module_select_row(Module *module, uint8_t row);

extern Module modules[NUM_MODULES];
extern const uint8_t row_to_module_map[MATRIX_ROWS];

#endif
