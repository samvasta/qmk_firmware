#include "quantum.h"
#include "module.h"

void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.

  // i2c_init();

  // Set our LED pins as output
  setPinOutput(D6);
  setPinOutput(D7);
  writePinHigh(D6);
  writePinHigh(D7);

  // uint8_t ret = i2c_start(EXPANDER_ADDR(0,1,0) | I2C_WRITE, I2C_TIMEOUT);
  // // writePinHigh(D7);
  // if(ret == 0){
  //   // It's connected
  //   // Low = on
  //   i2c_stop();
  //   writePinLow(D6);
  // }
  // else {
  //   // It's NOT connected
  //   // High = off
  //   writePinHigh(D6);
  // }

}
