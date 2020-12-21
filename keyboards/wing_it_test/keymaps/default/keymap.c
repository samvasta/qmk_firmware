#include QMK_KEYBOARD_H
#include "print.h"
#include "debug.h"

extern const uint16_t firmware_length;
extern const uint8_t firmware_data[];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	LAYOUT(KC_A),
};

void matrix_init_user(void) {
    print("Hello World!\n");
}

uint16_t count = 0;
void matrix_scan_user(void) {
    ++count;
    if(count % 1000 == 0){
        print("count\n");
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    print("pressed!\n");
	return true;
}


#ifdef OLED_DRIVER_ENABLE
extern int8_t pointer_dx;
extern int8_t pointer_dy;

uint8_t oledCounter1 = 0;
uint8_t oledCounter2 = 0;

void keyboard_post_init_user(void) {
    //Sometimes OLED doesn't turn on during "cold" boot. Waiting a bit helps for some reason
  wait_ms(1000);
  oled_init(OLED_ROTATION_0);
}

void oled_task_user(void) {

    // Host Keyboard Layer Status
    // oled_write_P(PSTR("Hello World!\n"), false);

    ++oledCounter1;
    if(oledCounter1 % 500 == 0) {
        oled_clear();
        ++oledCounter2;
        oledCounter1 = 0;
    }
    char number_str[10];
    sprintf(number_str, "%d", oledCounter2);
    oled_write_ln(number_str, false);


    // Host Keyboard LED Status
    // led_t led_state = host_keyboard_led_state();
    // oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    // oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    // oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    char pointer_str[15] = {0};
    sprintf(pointer_str, "x=%d", pointer_dx);
    oled_write_ln(pointer_str, false);
    sprintf(pointer_str, "y=%d", pointer_dy);
    oled_write_ln(pointer_str, false);
}
#endif
