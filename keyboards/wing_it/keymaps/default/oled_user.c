#include <string.h>
#include "oled_common.h"
#include "oled_driver.h"
#include "host.h"
#include "config.h"
#include "action_layer.h"
#include "wpm.h"
#include "progmem.h"
#include "matrix.h"

#define HEAT_MAP_DECAY_RATE 0.99
#define HEAT_MAP_ACCUMULATE_RATE 50
#define HEAT_MAP_STEP 42

const char layer_name[][8] = {
  "ALPHA",
  "NAVIG",
  "SYMBOL",
  "MEDIA",
  "NUMPAD",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----",
  "----"
};

enum DrawItem {
  WPM_CONTAINER = 0,
  WPM_BARS,
  LEFT_GRID,
  RIGHT_GRID,
  CAPS_LOCK_ICON,
  CAPS_INDICATOR,
  NUM_LOCK_ICON,
  NUM_INDICATOR,
  F_1,
  F_2,
  F_3,
  F_4,
  F_1_INDICATOR,
  F_2_INDICATOR,
  F_3_INDICATOR,
  F_4_INDICATOR,
} DrawItem;

uint32_t draw_states = 0xffffffff;

#define FLAG_FOR_REDRAW(item) draw_states |= (1<<item)
#define NEEDS_REDRAW(item) (1<<item & draw_states)
#define CLEAR_DRAW_FLAG(item) draw_states &= ~(1<<item)

led_t current_led_state;
uint8_t active_layer;
uint8_t wpm_bars;
uint8_t last_wpm_bars;
uint8_t key_heat_map[8*6];

void oled_pre_task(void) {
  // Update function layer states
  active_layer = 0;
  layer_state_t current_layer_state = layer_state;
  while(current_layer_state) { ++active_layer; current_layer_state = current_layer_state>>1; }
  --active_layer;

  if(active_layer & 1) {
    FLAG_FOR_REDRAW(F_1);
  }
  if(active_layer & 2) {
    FLAG_FOR_REDRAW(F_2);
  }
  if(active_layer & 4) {
    FLAG_FOR_REDRAW(F_3);
  }
  if(active_layer & 8) {
    FLAG_FOR_REDRAW(F_4);
  }

  // Update caps & num lock states
  led_t next_led_state = host_keyboard_led_state();
  if(next_led_state.caps_lock != current_led_state.caps_lock) {
    FLAG_FOR_REDRAW(CAPS_INDICATOR);
  }

  if(next_led_state.num_lock != current_led_state.num_lock) {
    FLAG_FOR_REDRAW(NUM_INDICATOR);
  }

  current_led_state = next_led_state;

  // Update WPM counter state
  last_wpm_bars = wpm_bars;
  wpm_bars = get_current_wpm() / 17;  // ~120wpm = 100%
  if(wpm_bars > 7) { wpm_bars = 7; }
  if(wpm_bars != last_wpm_bars) {
    FLAG_FOR_REDRAW(WPM_BARS);
  }

  // Update heat map based on current matrix state
  for(uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    matrix_row_t row_state = matrix_get_row(row);
    for(uint8_t col = 0; col < 6; ++col) {
      uint8_t *data = &key_heat_map[row*6+col];
      if(row_state & (1<<col)) {
        if(*data < 255-HEAT_MAP_ACCUMULATE_RATE){
          *data += HEAT_MAP_ACCUMULATE_RATE;
        }
        else {
          *data = 255;
        }
      } else if(*data > 0) {
        // --*data;
        *data *= HEAT_MAP_DECAY_RATE;
      }
    }
  }

  draw_states = 0xffffffff;
}


#define WPM_CONTAINER_X 101
#define WPM_CONTAINER_Y 1
#define LEFT_GRID_X 6
#define LEFT_GRID_Y 6

void oled_task_left(Oled *oled) {
  if(NEEDS_REDRAW(WPM_CONTAINER)) {
    draw_wpm_container(oled, WPM_CONTAINER_X, WPM_CONTAINER_Y);
    CLEAR_DRAW_FLAG(WPM_CONTAINER);
  }

  for(uint8_t row = 0; row < MAX_MODULE_ROWS; ++row) {
    for(uint8_t col = 0; col < 6; ++col) {
      fill_switch_grid_cell(oled, LEFT_GRID_X, LEFT_GRID_Y, col, row, key_heat_map[row*6+col] / HEAT_MAP_STEP);
    }
  }

  if(NEEDS_REDRAW(WPM_BARS)) {
    for(uint8_t bar = 0; bar < 8; bar++) {
        draw_wpm_level_marker(oled, WPM_CONTAINER_X, WPM_CONTAINER_Y, bar, wpm_bars > bar);
    }
  }

  if(NEEDS_REDRAW(LEFT_GRID)) {
    draw_switch_grid(oled, LEFT_GRID_X, LEFT_GRID_Y);
    CLEAR_DRAW_FLAG(LEFT_GRID);
  }

}


#define CAPS_INDICATOR_X 0
#define CAPS_INDICATOR_Y 6
#define CAPS_ICON_X 9
#define CAPS_ICON_Y 6

#define NUM_INDICATOR_X 0
#define NUM_INDICATOR_Y 20
#define NUM_ICON_X 9
#define NUM_ICON_Y 20

#define FUNC_INDICATOR_START_X 0
#define FUNC_INDICATOR_START_Y 35

#define RIGHT_GRID_X 43
#define RIGHT_GRID_Y 6

void oled_task_right(Oled *oled) {
  char buffer[8];
  memset(buffer, 0, sizeof(buffer));
  memcpy(buffer, layer_name[active_layer], 8);
  oled_write_P(PSTR("\n\n\n\n\n\n"), false, oled);
  oled_write(buffer, false, oled);

  // Update Switch Grid
  if(NEEDS_REDRAW(RIGHT_GRID)) {
    draw_switch_grid(oled, RIGHT_GRID_X, RIGHT_GRID_Y);
    CLEAR_DRAW_FLAG(RIGHT_GRID);
  }

  for(uint8_t row = 0; row < MAX_MODULE_ROWS; ++row) {
    for(uint8_t col = 0; col < 6; ++col) {
      fill_switch_grid_cell(oled, RIGHT_GRID_X, RIGHT_GRID_Y, 5-col, row, key_heat_map[(row+4)*6+col] / HEAT_MAP_STEP);
    }
  }

  // Draw caps lock
  if(NEEDS_REDRAW(CAPS_INDICATOR)) {
    if(current_led_state.caps_lock) {
      draw_lock_closed(oled, CAPS_INDICATOR_X, CAPS_INDICATOR_Y);
    }
    else {
      draw_lock_open(oled, CAPS_INDICATOR_X, CAPS_INDICATOR_Y);
    }
    CLEAR_DRAW_FLAG(CAPS_INDICATOR);
  }
  if(NEEDS_REDRAW(CAPS_LOCK_ICON)) {
    draw_caps_lock(oled, CAPS_ICON_X, CAPS_ICON_Y);
    CLEAR_DRAW_FLAG(CAPS_LOCK_ICON);
  }

  // Draw num lock
  if(NEEDS_REDRAW(NUM_INDICATOR)) {
    if(current_led_state.num_lock) {
      draw_lock_closed(oled, NUM_INDICATOR_X, NUM_INDICATOR_Y);
    }
    else {
      draw_lock_open(oled, NUM_INDICATOR_X, NUM_INDICATOR_Y);
    }
    CLEAR_DRAW_FLAG(NUM_INDICATOR);
  }
  if(NEEDS_REDRAW(NUM_LOCK_ICON)) {
    draw_num_lock(oled, NUM_ICON_X, NUM_ICON_Y);
    CLEAR_DRAW_FLAG(NUM_LOCK_ICON);
  }

  // Draw function indicators
  if(NEEDS_REDRAW(F_1_INDICATOR)) {
    draw_func_indicator(oled, FUNC_INDICATOR_START_X, FUNC_INDICATOR_START_Y);
    CLEAR_DRAW_FLAG(F_1_INDICATOR);
  }
  if(NEEDS_REDRAW(F_2_INDICATOR)) {
    draw_func_indicator(oled, FUNC_INDICATOR_START_X + 10, FUNC_INDICATOR_START_Y);
    CLEAR_DRAW_FLAG(F_2_INDICATOR);
  }
  if(NEEDS_REDRAW(F_3_INDICATOR)) {
    draw_func_indicator(oled, FUNC_INDICATOR_START_X + 20, FUNC_INDICATOR_START_Y);
    CLEAR_DRAW_FLAG(F_3_INDICATOR);
  }
  if(NEEDS_REDRAW(F_4_INDICATOR)) {
    draw_func_indicator(oled, FUNC_INDICATOR_START_X + 30, FUNC_INDICATOR_START_Y);
    CLEAR_DRAW_FLAG(F_4_INDICATOR);
  }
  if(NEEDS_REDRAW(F_1)) {
    fill_in_func_indicator(oled, FUNC_INDICATOR_START_X, FUNC_INDICATOR_START_Y, active_layer & 1);
    CLEAR_DRAW_FLAG(F_1);
  }
  if(NEEDS_REDRAW(F_2)) {
    fill_in_func_indicator(oled, FUNC_INDICATOR_START_X + 10, FUNC_INDICATOR_START_Y, active_layer & 2);
    CLEAR_DRAW_FLAG(F_2);
  }
  if(NEEDS_REDRAW(F_3)) {
    fill_in_func_indicator(oled, FUNC_INDICATOR_START_X + 20, FUNC_INDICATOR_START_Y, active_layer & 4);
    CLEAR_DRAW_FLAG(F_3);
  }
  if(NEEDS_REDRAW(F_4)) {
    fill_in_func_indicator(oled, FUNC_INDICATOR_START_X + 30, FUNC_INDICATOR_START_Y, active_layer & 8);
    CLEAR_DRAW_FLAG(F_4);
  }
}
