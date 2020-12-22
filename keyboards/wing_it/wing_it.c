#include "quantum.h"
#include "module.h"

void keyboard_pre_init_user(void) {
  //Wait so the debug console can pick up the device
  wait_ms(5000);
  debug_enable=true;
  debug_matrix=true;
  // Call the keyboard pre init code.

  // i2c_init();

  // Set our LED pins as output
  setPinOutput(D6);
  setPinOutput(B4);
  writePinHigh(D6);
  writePinHigh(B4);
  wait_ms(1000);

  writePinLow(D6);
  writePinLow(B4);

  // uint8_t ret = i2c_start(EXPANDER_ADDR(0,0,0) | I2C_WRITE, I2C_TIMEOUT);
  // writePinHigh(D7);
  // if(ret == 0){
  //   i2c_stop();
  //   writePinHigh(D6);
  // }
  // else {
  //   writePinLow(D6);
  // }

}
