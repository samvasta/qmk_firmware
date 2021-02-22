#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

/* USB Device descriptor parameter */

#define VENDOR_ID       0x5356 // "SV"
#define PRODUCT_ID      0x0001 // sequentially numbered

#define DEVICE_VER      0x0001
#define MANUFACTURER    Sam Vasta
#define PRODUCT         Wing_it!
#define DESCRIPTION     QMK firmware for the "Wing_it!" keyboard


/* key matrix size */
#define NUM_MODULES 2
#define MAX_MODULE_ROWS 4
#define MAX_MODULE_COLS 8

#define MATRIX_ROWS 8
#define MATRIX_COLS 8

#define MOUSEKEY_INTERVAL       20
#define MOUSEKEY_DELAY          0
#define MOUSEKEY_TIME_TO_MAX    60
#define MOUSEKEY_MAX_SPEED      7
#define MOUSEKEY_WHEEL_DELAY    0

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

#define USB_MAX_POWER_CONSUMPTION 500

#define NUM_OLEDS 2
#define OLED_DISPLAY_128X64
#define OLED_FRAME_SKIP 10

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION
//#define DEBUG_MATRIX_SCAN_RATE

#endif
