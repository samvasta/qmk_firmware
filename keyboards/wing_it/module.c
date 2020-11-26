#include <stdbool.h>
#include "action.h"
#include "module.h"
#include "debug.h"

void expander_config(Module *module);
uint8_t expander_write(Module *module, uint8_t reg, uint8_t data);
uint8_t expander_read(Module *module, uint8_t reg, uint8_t *data);


void expander_config(Module *module)
{
  // Set polarity of port A to %11111111
  // 1 = GPIO register bit reflects the opposite logic state of the input pin.
  // 0 = GPIO register bit reflects the same logic state of the input pin.
  expander_write(module, EXPANDER_REG_IPOLA, 0xFF);

  // Activate all pull-up resistors on port A
  // PU<7:0> Controls the weak pull-up resistors on each pin (when configured as an input)
  // 1 = Pull-up enabled
  // 0 = Pull-up disabled
  expander_write(module, EXPANDER_REG_GPPUA, 0xFF);

  // enable pull-up resistors on port B's top 4 bits (bit 5 is used to drive an LED)
  expander_write(module, EXPANDER_REG_GPPUB, 0xF0);

  // Set the I/O direction on port B to Input for the bottom 4 bits
  // Essentially disabling reading the state of port B
  // 1 = Pin is configured as an input.
  // 0 = Pin is configured as an output.
  expander_write(module, EXPANDER_REG_IODIRB, 0x0F);

  // Port A is set to Input implicitly since input is the default state
}

uint8_t expander_write(Module *module, uint8_t reg, uint8_t data)
{
  if (module->status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(module->address | I2C_WRITE, I2C_TIMEOUT);
  if (ret)
  {
    goto stop;
  }

  ret = i2c_write(reg, I2C_TIMEOUT);
  if (ret)
  {
    goto stop;
  }

  ret = i2c_write(data, I2C_TIMEOUT);

 stop:
  i2c_stop();
  return ret;
}

uint8_t expander_read(Module *module, uint8_t reg, uint8_t *data)
{
  if (module->status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(module->address | I2C_WRITE, I2C_TIMEOUT);
  if (ret)
  {
    goto stop;
  }

  ret = i2c_write(reg, I2C_TIMEOUT);
  if (ret)
  {
    goto stop;
  }

  ret = i2c_start(module->address | I2C_READ, I2C_TIMEOUT);
  if (ret)
  {
    goto stop;
  }

  *data = i2c_read_nack(I2C_TIMEOUT);

 stop:
  i2c_stop();
  return ret;
}


static bool i2c_initialized = 0;
void module_init(Module *module){
  if(!i2c_initialized){
    i2c_init();
  }
  module_scan(module);
}

void module_scan(Module *module){

  dprintf("expander status: %d ... ", module->status);
  uint8_t ret = i2c_start(module->address | I2C_WRITE, I2C_TIMEOUT);
  if (ret == 0) {
    i2c_stop();
    if (module->status == 0) {
      dprintf("attached\n");
      module->status = 1;
      expander_config(module);
    }
  }
  else {
    if (module->status == 1) {
      dprintf("detached\n");
      module->status = 0;
    }
  }
  dprintf("%d\n", module->status);
}

matrix_row_t module_read_cols(Module *module){
  matrix_row_t data = 0;
  // Read value of all 8 pins of port A
  expander_read(module, EXPANDER_REG_GPIOA, &data);
  return data;
}

void module_unselect_rows(Module *module){
  // Set the I/O direction on port B to Input for the bottom 4 bits
  // Essentially disabling reading the state of port B
  // 1 = Pin is configured as an input.
  // 0 = Pin is configured as an output.
  expander_write(module, EXPANDER_REG_IODIRB, 0xFF);
}

void module_select_row(Module *module, uint8_t row){
  // Set the I/O direction on port B to input on all except one pin,
  // determined by the row. The row pin will be output, so we can read the value
  // 1 = Pin is configured as an input.
  // 0 = Pin is configured as an output.
  expander_write(module, EXPANDER_REG_IODIRB, ~(1<<row));
}
