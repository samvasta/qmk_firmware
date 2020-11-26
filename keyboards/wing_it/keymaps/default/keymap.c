#include QMK_KEYBOARD_H
#include "config.h"
#include "module.h"
#include "wing_it.h"
#include "oled_driver.h"

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys

enum custom_keycodes {
  VRSN = SAFE_RANGE,
  RGB_SLD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   =    |   1  |   2  |   3  |   4  |   5  | LEFT |           | RIGHT|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Del    |   Q  |   W  |   E  |   R  |   T  |  L1  |           |  L1  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | BkSp   |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |; / L2|' / Cmd |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |Z/Ctrl|   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |//Ctrl| RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |Grv/L1|  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | ~L1  |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | App  | LGui |       | Alt  |Ctrl/Esc|
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      | Home |       | PgUp |        |      |
 *                                 | Space|Backsp|------|       |------|  Tab   |Enter |
 *                                 |      |ace   | End  |       | PgDn |        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT( // layer 0 : default
        KC_A, KC_B,
        KC_C, KC_D
    ),
};

#define MODULE(address, colA, colB, rowA, rowB) (Module){.status=0,.address=address,.num_cols=2,.num_rows=2 \
.col_map={colA,colB}, .row_map={rowA, rowB}}

const uint8_t moduleAAddress = EXPANDER_ADDR(1,0,0);

Module modules[NUM_MODULES] = {
  {
    .address=EXPANDER_ADDR(1,0,0),
    .num_rows=2,
    .num_cols=2,
    .col_map={
      EXPANDER_A1,
      EXPANDER_A2,
    },
    .row_map = {
      EXPANDER_B1,
      EXPANDER_B2
    }
  }
  // MODULE(moduleAAddress, EXPANDER_A1, EXPANDER_A2, EXPANDER_B1, EXPANDER_B2)
};

const uint8_t PROGMEM row_to_module_map[] = {
  0,0
};


void oled_task_user(Oled *oled) {
  if(oled->address == 0x3C) {
    // Host Keyboard Layer Status
    oled_write_ln_P(PSTR("0x3C!"), false, oled);
  }
  else if(oled->address == 0x3D) {
    // Host Keyboard Layer Status
    oled_write_ln_P(PSTR("0x3D!"), false, oled);
  }

    // // Host Keyboard LED Status
    // led_t led_state = host_keyboard_led_state();
    // oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false, oled);
    // oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false, oled);
    // oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false, oled);
}


Oled Oleds[NUM_OLEDS] = {
  // new_Oled(0x3C), new_Oled(0x3D)
  {
    .address = 0x3C,
    .dirty          = 0,
    .initialized    = false,
    .active         = false,
    .scrolling      = false,
    .brightness     = OLED_BRIGHTNESS,
    .rotation       = 0,
    .rotation_width = 0,
    .scroll_speed   = 0,  // this holds the speed after being remapped to ssd1306 internal values
    .scroll_start   = 0,
    .scroll_end     = 7
  },
  {
    .address = 0x3D,
    .dirty          = 0,
    .initialized    = false,
    .active         = false,
    .scrolling      = false,
    .brightness     = OLED_BRIGHTNESS,
    .rotation       = 0,
    .rotation_width = 0,
    .scroll_speed   = 0,  // this holds the speed after being remapped to ssd1306 internal values
    .scroll_start   = 0,
    .scroll_end     = 7
  }
};
