#include QMK_KEYBOARD_H
#include "config.h"
#include "module.h"
#include "wing_it.h"
#include "oled_driver.h"

#define ______ KC_TRNS

#define BASE_LAYER 0x0		//FN			    (0 btns)
#define LAYER_1    0x1		//FN 1			  (1 btns) *
#define LAYER_2    0x2		//FN   2		  (1 btns) *
#define LAYER_3    0x3		//FN 1 2		  (2 btns) **
#define LAYER_4    0x4		//FN     3		(1 btns) *
#define LAYER_5    0x5		//FN 1   3		(2 btns) **
#define LAYER_6    0x6		//FN   2 3		(2 btns) **
#define LAYER_7    0x7		//FN 1 2 3		(3 btns) ***
#define LAYER_8    0x8		//FN       4	(1 btns) *
#define LAYER_9    0x9		//FN 1     4	(2 btns) **
#define LAYER_A    0xA		//FN   2   4	(2 btns) **
#define LAYER_B    0xB		//FN 1 2   4	(3 btns) ***
#define LAYER_C    0xC		//FN     3 4	(2 btns) **
#define LAYER_D    0xD		//FN 1   3 4	(3 btns) ***
#define LAYER_E    0xE		//FN   2 3 4	(3 btns) ***
#define LAYER_F    0xF		//FN 1 2 3 4	(4 btns) ****


enum custom_keycodes {
  UNICD = SAFE_RANGE, // Unicode momentary entry mode
  FN1,
  FN2,
  FN4,
  FN8
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
	BASE LAYER (no FN)
	Left Hand                                                 L. Thumb                              R. Thumb    Right Hand
	┌────────┬────────┬────────┬────────┬────────┬────────┐                                                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
	│  ESC   │    Q   │   W    │   E    │   R    │   T    │                                                    │    Y   │   U    │   I    │    O   │   P    │ BKSPC  │
	├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	│  TAB   │    A   │   S    │   D    │   F    │   G    |                                                    │    H   │   J    │   K    │    L   │   ;    │ ENTER  │
	├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	│  SHIFT │    Z   │   X    │   C    │   V    │   B    │                                                    │    N   │   M    │   , <  │  . >   │   / ?  │ R SHFT │
	├────────┼────────┼────────┼────────┼────────┼────────┤  ┌────────┐┌────────┐        ┌────────┐┌────────┐  ├────────┼────────┼────────┼────────┼────────┼────────┤
	│  CTL   │  GUI   │  ALT   │  FN 8  │  FN 2  │  SPACE │  │  FN 1  ││  FN 4  │        │  FN 8  ││  FN 2  │  │ SPACE  │  FN 1  │  FN 4  │  ALT   │  GUI   │  CTL   │
	└────────┴────────┴────────┴────────┴────────┴────────┘  └────────┘└────────┘        └────────┘└────────┘  └────────┴────────┴────────┴────────┴────────┴────────┘
	*/
[BASE_LAYER] = LAYOUT( // layer 0 : default
        // KC_A,    KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,                                                  KC_Y,     KC_U,  KC_I,     KC_O,    KC_P,     KC_A,
        // KC_A,    KC_A,    KC_S,    KC_D,  KC_F,  KC_G,                                                  KC_H,     KC_J,  KC_K,     KC_L,    KC_A,  KC_A,
        // KC_A, KC_Z,    KC_X,    KC_C,  KC_V,  KC_B,                                                  KC_N,     KC_M,  KC_A, KC_A,  KC_A, KC_A,
        // KC_A,  KC_A, KC_A, KC_A,   KC_A,   KC_A, KC_A,   KC_A,                      KC_A, KC_A,   KC_A, KC_A,   KC_A,      KC_A, KC_A,  KC_A


        KC_ESC,    KC_Q,    KC_W,    KC_E,  KC_R,  KC_T,                                                  KC_Y,     KC_U,  KC_I,     KC_O,    KC_P,     KC_BSPC,
        KC_TAB,    KC_A,    KC_S,    KC_D,  KC_F,  KC_G,                                                  KC_H,     KC_J,  KC_K,     KC_L,    KC_SCLN,  KC_ENTER,
        KC_LSHIFT, KC_Z,    KC_X,    KC_C,  KC_V,  KC_B,                                                  KC_N,     KC_M,  KC_COMMA, KC_DOT,  KC_SLASH, KC_RSHIFT,
        KC_LCTRL,  KC_LGUI, KC_LALT, FN8,   FN2,   KC_SPACE, FN1,   FN4,                      FN8, FN2,   KC_SPACE, FN1,   FN4,      KC_RALT, KC_RGUI,  KC_RCTRL

    )

	// /*
	// LAYER 1 (FN 1)
	// Left Hand                                                 L. Thumb                              R. Thumb    Right Hand
	// ┌────────┬────────┬────────┬────────┬────────┬────────┐                                                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
	// │        │ XXXXXX │ XXXXXX │ XXXXXX │  HOME  │ PG.UP  │                                                    │ XXXXXX │ BKSPC  │   UP   │  DEL   │ INSERT │ PRTSCR │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │SHFT+TAB│ L CTL  │ L ALT  │   END  │ PG.DN  │                                                    │ XXXXXX │ LEFT   │  DOWN  │  RIGHT │ XXXXXX │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │ XXXXXX │ XXXXXX │ XXXXXX │ XXXXXX │ XXXXXX │                                                    │ XXXXXX │ CUT    │  COPY  │ PASTE  │ XXXXXX │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤  ┌────────┐┌────────┐        ┌────────┐┌────────┐  ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │  │        ││        │        │        ││        │  │        │        │        │        │        │        │
	// └────────┴────────┴────────┴────────┴────────┴────────┘  └────────┘└────────┘        └────────┘└────────┘  └────────┴────────┴────────┴────────┴────────┴────────┘
	// */
	// [LAYER_1] = LAYOUT(
	// 	______, KC_NO,     KC_NO,   KC_NO,   KC_HOME, KC_PGUP,                                                  KC_NO , KC_BSPC, KC_UP,   KC_DEL,   KC_INS, KC_PSCR,
	// 	______, S(KC_TAB), KC_LCTL, KC_LALT, KC_END,  KC_PGDN,                                                  KC_NO , KC_LEFT, KC_DOWN, KC_RIGHT, KC_NO , ______,
	// 	______, KC_NO,     KC_NO,   KC_NO,   KC_NO,   KC_NO,                                                    KC_NO , KC_CUT,  KC_COPY, KC_PASTE, KC_NO , ______,
	// 	______, ______,    ______,  ______,  ______,  ______,  ______, ______,                ______,  ______,  ______, ______,  ______,  ______,   ______, ______
	// ),

	// /*
	// LAYER 2 (FN 2)
	// Left Hand                                                 L. Thumb                              R. Thumb    Right Hand
	// ┌────────┬────────┬────────┬────────┬────────┬────────┐                                                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
	// │  ` ~   │    !   │    @   │   #    │   $    │   %    │                                                    │   ^    │   &    │   *    │   (    │    )   │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │ XXXXXX │    |   │   _    │   +    │   {    │                                                    │   }    │   "    │   <    │   >    │   ?    │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │ XXXXXX │    \   │   -    │   =    │   [    │                                                    │   ]    │   '    │   ,    │   .    │   /    │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤  ┌────────┐┌────────┐        ┌────────┐┌────────┐  ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │  │        ││        │        │        ││        │  │        │        │        │        │        │        │
	// └────────┴────────┴────────┴────────┴────────┴────────┘  └────────┘└────────┘        └────────┘└────────┘  └────────┴────────┴────────┴────────┴────────┴────────┘
	// */
	// [LAYER_2] = LAYOUT(
	// 	KC_GRV, KC_EXLM, KC_AT ,       KC_HASH,	   KC_DLR,    KC_PERC,                                           KC_CIRC   , KC_AMPR,    KC_ASTR,    KC_LPRN,   KC_RPRN,     KC_BSPC,
	// 	______, KC_NO  , S(KC_BSLASH), S(KC_UNDS), S(KC_EQL), S(KC_LBRC) ,                                       S(KC_RBRC), S(KC_QUOT), S(KC_COMM), S(KC_DOT), S(KC_SLASH), ______,
	// 	______, KC_NO  , KC_BSLASH,    KC_UNDS,    KC_EQL,    KC_LBRC ,                                          KC_RBRC   , KC_QUOT ,   KC_COMM,    KC_DOT,    KC_SLASH,    ______,
	// 	______, ______ , ______,       ______ ,    ______,    ______ ,     ______, ______,     ______,  ______,  ______    , ______ ,    ______ ,    ______ ,   ______ ,     ______
	// ),


	// /*
	// LAYER 3 (FN 1+2)
	// Left Hand                                                 L. Thumb                              R. Thumb    Right Hand
	// ┌────────┬────────┬────────┬────────┬────────┬────────┐                                                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
	// │        │ VOL-   │  MUTE  │  VOL+  │ XXXXXX │ XXXXXX │                                                    │ XXXXXX │  F1    │  F2    │  F3    │  F4    │    F5  │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │  <<    │   > || │  >>    │ XXXXXX │ XXXXXX │                                                    │ XXXXXX │  F6    │  F7    │  F8    │  F9    │   F10  │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │ XXXXXX │ XXXXXX │ XXXXXX │ XXXXXX │ XXXXXX │                                                    │ XXXXXX │  F11   │  F12   │  F13   │  F14   │   F15  │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤  ┌────────┐┌────────┐        ┌────────┐┌────────┐  ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │  │        ││        │        │        ││        │  │        │        │        │        │        │        │
	// └────────┴────────┴────────┴────────┴────────┴────────┘  └────────┘└────────┘        └────────┘└────────┘  └────────┴────────┴────────┴────────┴────────┴────────┘
	// */
	// [LAYER_3] = LAYOUT(
	// 	______, KC_VOLD, KC_MUTE, KC_VOLU, KC_NO , KC_NO ,                                                        KC_NO , KC_F1 , KC_F2 , KC_F3 , KC_F4 , ______,
	// 	______, KC_MPRV, KC_MPLY, KC_MNXT, KC_NO , KC_NO ,                                                        KC_NO , KC_F5 , KC_F6 , KC_F7 , KC_F8 , ______,
	// 	______, KC_NO ,  KC_NO ,  KC_NO ,  KC_NO , KC_NO ,                                                        KC_NO , KC_F9 , KC_F10, KC_F11, KC_F12, ______,
	// 	______, ______,  ______,  ______,  ______, ______,   ______, ______,                   ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// /*
	// LAYER 4 (FN 4)
	// Left Hand                                                 L. Thumb                              R. Thumb    Right Hand
	// ┌────────┬────────┬────────┬────────┬────────┬────────┐                                                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
	// │        │        │ NUMLCK │ U TGL  │ L ALT  │        │                                                    │ NUM 0  │ NUM 1  │ NUM 2  │ NUM 3  │ NUM 4  │ NUM 5  │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │   *    │   /    │   _    │   +    │ NUM .  │                                                    │   x    │ NUM 6  │ NUM 7  │ NUM 8  │ NUM 9  │    A   │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │   -    │   =    │        │                                                    │   #    │    B   │    C   │    D   │   E    │    F   │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤  ┌────────┐┌────────┐        ┌────────┐┌────────┐  ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │  │        ││        │        │        ││        │  │        │        │        │        │        │        │
	// └────────┴────────┴────────┴────────┴────────┴────────┘  └────────┘└────────┘        └────────┘└────────┘  └────────┴────────┴────────┴────────┴────────┴────────┘
	// */
	// [LAYER_4] = LAYOUT(
	// 	______, ______,  KC_NUMLOCK, UNICD,    KC_LALT,    ______,                                                KC_P0,   KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,
	// 	______, KC_ASTR, KC_SLASH,   KC_UNDS,  KC_KP_PLUS, KC_PDOT,                                               KC_X,    KC_P6,   KC_P7,   KC_P8,   KC_P9,   S(KC_A),
	// 	______, ______,  ______,     KC_MINUS, KC_EQL,     ______,                                                KC_HASH, S(KC_B), S(KC_C), S(KC_D), S(KC_E), S(KC_F),
	// 	______, ______,  ______,     ______,   ______,     ______,   ______, ______,              ______, ______, ______,  ______,  ______,  ______,  ______,  ______
	// ),

	// /*
	// LAYER 5 (FN 1+4)
	// Left Hand                                                 L. Thumb                              R. Thumb    Right Hand
	// ┌────────┬────────┬────────┬────────┬────────┬────────┐                                                    ┌────────┬────────┬────────┬────────┬────────┬────────┐
	// │        │        │        │        │        │        │                                                    │        │        │        │        │        │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │                                                    │        │        │        │        │        │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤                                                    ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │                                                    │        │        │        │        │        │        │
	// ├────────┼────────┼────────┼────────┼────────┼────────┤  ┌────────┐┌────────┐        ┌────────┐┌────────┐  ├────────┼────────┼────────┼────────┼────────┼────────┤
	// │        │        │        │        │        │        │  │        ││        │        │        ││        │  │        │        │        │        │        │        │
	// └────────┴────────┴────────┴────────┴────────┴────────┘  └────────┘└────────┘        └────────┘└────────┘  └────────┴────────┴────────┴────────┴────────┴────────┘
	// */
	// [LAYER_5] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 2+4
	// [LAYER_6] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 1+2+4
	// [LAYER_7] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 8
	// [LAYER_8] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 1+8
	// [LAYER_9] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 2+8
	// [LAYER_A] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 1+2+8
	// [LAYER_B] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 4+8
	// [LAYER_C] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 1+4+8
	// [LAYER_D] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 2+4+8
	// [LAYER_E] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// ),

	// //FN 1+2+4+8
	// [LAYER_F] = LAYOUT(
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,                                          ______, ______, ______, ______, ______, ______,
	// 	______, ______, ______, ______, ______, ______,   ______, ______,     ______,  ______,   ______, ______, ______, ______, ______, ______
	// )
};

bool unicode_active = false;
uint8_t unicode_pos = 0;
char unicode_buff[4];

void dump_unicode(void) {
  if(unicode_pos == 4) {
    send_unicode_hex_string(unicode_buff);
  }
  unicode_buff[0] = 0;
  unicode_buff[1] = 0;
  unicode_buff[2] = 0;
  unicode_buff[3] = 0;
}

void on_fn(uint8_t mask, bool pressed) {
  if(pressed){
    layer_or(mask);
  }
  else {
    layer_and(~mask);
  }
}

// Sue me
#define is_hex_digit(keycode) (keycode == KC_0 || keycode == KC_1 || keycode == KC_2 || keycode == KC_3 || keycode == KC_4 || keycode == KC_5 || keycode == KC_6 || keycode == KC_7 || keycode == KC_8 || keycode == KC_9 || keycode == KC_A || keycode == KC_B || keycode == KC_C || keycode == KC_D || keycode == KC_E || keycode == KC_F)

char get_char(uint16_t keycode){
  switch(keycode) {
    case KC_0:
      return '0';
    case KC_1:
      return '1';
    case KC_2:
      return '2';
    case KC_3:
      return '3';
    case KC_4:
      return '4';
    case KC_5:
      return '5';
    case KC_6:
      return '6';
    case KC_7:
      return '7';
    case KC_8:
      return '8';
    case KC_9:
      return '9';
    case KC_A:
      return 'A';
    case KC_B:
      return 'B';
    case KC_C:
      return 'C';
    case KC_D:
      return 'D';
    case KC_E:
      return 'E';
    case KC_F:
      return 'F';
    default:
      return '0';
  }
}

uint16_t last_keycode = 0;
keypos_t last_pos;
bool has_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // writePinLow(D6);
    has_pressed = record->event.pressed;
    last_keycode = keycode;
    last_pos = record->event.key;
    return true;
  // if(record->event.pressed){
  // }
  // switch (keycode) {
  //   case UNICD:
  //     if (record->event.pressed) {
  //       unicode_active = true;
  //     } else {
  //       dump_unicode();
  //       unicode_active = false;
  //     }
  //     return false;
  //   case FN1:
  //     on_fn(1, record->event.pressed);
  //     return false; // Let QMK send the enter press/release events
  //   case FN2:
  //     on_fn(2, record->event.pressed);
  //     return false; // Let QMK send the enter press/release events
  //   case FN4:
  //     on_fn(4, record->event.pressed);
  //     return false; // Let QMK send the enter press/release events
  //   case FN8:
  //     on_fn(8, record->event.pressed);
  //     return false; // Let QMK send the enter press/release events
  //   default:
  //     if(unicode_active){
  //       if(record->event.pressed && is_hex_digit(keycode)){
  //         unicode_buff[unicode_pos++] = get_char(keycode);
  //         if(unicode_pos >= 4){
  //           dump_unicode();
  //         }
  //       }
  //       return false; // We're buffering the key stroke so don't want to press the key
  //     }
  //     else {
  //       return true; // Process all other keycodes normally
  //     }
  // }
}


Module modules[NUM_MODULES] = {
  // Left Hand
  {
    .address=EXPANDER_ADDR(0,1,0),
    .num_rows=4,
    .num_cols=8,
    .col_map={
      EXPANDER_B1,
      EXPANDER_B2,
      EXPANDER_B3,
      EXPANDER_B4,
      EXPANDER_B5,
      EXPANDER_B6,
      EXPANDER_B7,
      EXPANDER_B8,
    },
    .row_map = {
      EXPANDER_A1,
      EXPANDER_A2,
      EXPANDER_A3,
      EXPANDER_A4
    }
  },

  // Right Hand
  {
    .address=EXPANDER_ADDR(1,0,0),
    .num_rows=4,
    .num_cols=8,
    .col_map={
      EXPANDER_B1,
      EXPANDER_B2,
      EXPANDER_B3,
      EXPANDER_B4,
      EXPANDER_B5,
      EXPANDER_B6,
      EXPANDER_B7,
      EXPANDER_B8,
    },
    .row_map = {
      EXPANDER_A1,
      EXPANDER_A2,
      EXPANDER_A3,
      EXPANDER_A4
    }
  }
};

const uint8_t row_to_module_map[MATRIX_ROWS] = {
  0,0,0,0,
  1,1,1,1
};


char buffer[10];
uint8_t count = 0;
void oled_task_user(Oled *oled) {
  if(oled->address == 0x3C) {
    // Host Keyboard Layer Status
    oled_write_ln_P(PSTR("0x3C!"), false, oled);

    oled_write_char(48+(count/10), false, oled);
    oled_write_ln_P(PSTR(""), false, oled);
    ++count;
    if(count > 99) {
      count = 0;
    }

    uint8_t row = last_pos.row;
    uint8_t col = last_pos.col;

    oled_write_P(PSTR("Last key: "), false, oled);
    oled_write_char(row+48, false, oled);
    oled_write_char(',', false, oled);
    oled_write_char(col+48, false, oled);
    oled_write_char(has_pressed ? '!' : '?', false, oled);
    oled_write_ln_P(PSTR(""), false, oled);

    // sprintf(buffer, "%d,%d", row, col);
    for(uint8_t i = 0; i < NUM_MODULES; i++) {
      Module module = modules[i];

      oled_write_char(i+48, false, oled);

      uint8_t ret = i2c_start(module.address | I2C_WRITE, I2C_TIMEOUT);
      if(ret == 0){
        i2c_stop();
        oled_write_ln_P(PSTR(" connected"), false, oled);
      }
      else {
        oled_write_ln_P(PSTR(" disconnected"), false, oled);
      }
    }


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
  // {
  //   .address = 0x3D,
  //   .dirty          = 0,
  //   .initialized    = false,
  //   .active         = false,
  //   .scrolling      = false,
  //   .brightness     = OLED_BRIGHTNESS,
  //   .rotation       = 0,
  //   .rotation_width = 0,
  //   .scroll_speed   = 0,  // this holds the speed after being remapped to ssd1306 internal values
  //   .scroll_start   = 0,
  //   .scroll_end     = 7
  // }
};
