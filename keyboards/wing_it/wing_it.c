#include "quantum.h"

void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.

  // Set our LED pins as output
  setPinOutput(F7);
  writePinHigh(F7);
}
