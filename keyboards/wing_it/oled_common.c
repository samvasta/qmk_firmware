#include <stdio.h>
#include <stdint.h>
#include "oled_common.h"
#include "oled_driver.h"
#include "config.h"

inline
void draw_horizontal_line(Oled *oled, uint8_t x, uint8_t y, uint8_t len, bool on) {
  for(uint8_t i = 0; i < len; i++) {
    oled_write_pixel(x+i, y, on, oled);
  }
}

inline
void draw_vertical_line(Oled *oled, uint8_t x, uint8_t y, uint8_t len, bool on) {
  for(uint8_t i = 0; i < len; i++) {
    oled_write_pixel(x, y+i, on, oled);
  }
}


inline
bool get_pixel(uint16_t index, const uint8_t *data)
{
  uint8_t chunk = data[index/8];
  uint8_t mask = 1<<(7-index%8);
  return chunk & mask;
}

void draw_data_arr(Oled *oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *data, uint8_t data_len) {
  for(uint8_t cur_y = 0; cur_y < height; cur_y++) {
    for(uint8_t cur_x = 0; cur_x < width; cur_x++) {
      oled_write_pixel(cur_x + x, cur_y+y, get_pixel(cur_y * width + cur_x, data), oled);
    }
  }
};




#define LOCK_WIDTH 7
#define LOCK_HEIGHT 11

const uint8_t lock_closed_data[10] = {
  0x00, 0x71, 0x12, 0x24, 0x5F, 0xFF, 0xF7, 0xEF, 0xFF, 0xFF
};

const uint8_t lock_open_data[10] = {
  0x38, 0x89, 0x12, 0x04, 0x0F, 0xA0, 0xC9, 0x93, 0x07, 0xFF
};

void draw_lock_open(Oled *oled, uint8_t x, uint8_t y) {
  draw_data_arr(oled, x, y, LOCK_WIDTH, LOCK_HEIGHT, &lock_open_data[0], sizeof(lock_open_data));
}

void draw_lock_closed(Oled *oled, uint8_t x, uint8_t y) {
  draw_data_arr(oled, x, y, LOCK_WIDTH, LOCK_HEIGHT, &lock_closed_data[0], sizeof(lock_closed_data));
}


#define CAPS_LOCK_WIDTH 15
#define CAPS_LOCK_HEIGHT 11
const uint8_t caps_lock_data[21] = {
  0x01, 0x00, 0x06, 0x00, 0x1C, 0x00, 0x78,
  0x01, 0xF3, 0x87, 0x68, 0x8C, 0xC1, 0x3F,
  0x9E, 0x73, 0x44, 0xE6, 0x9B, 0xCC, 0xDF
};
void draw_caps_lock(Oled *oled, uint8_t x, uint8_t y) {
  draw_data_arr(oled, x, y, CAPS_LOCK_WIDTH, CAPS_LOCK_HEIGHT, &caps_lock_data[0], sizeof(caps_lock_data));
}

#define NUM_LOCK_WIDTH 15
#define NUM_LOCK_HEIGHT 11
const uint8_t num_lock_data[21] = {
  0x06, 0x18, 0x1C, 0x70, 0x30, 0xC3, 0xFF,
  0xF0, 0xC3, 0x03, 0x8E, 0x06, 0x18, 0x7F,
  0xFC, 0x18, 0x60, 0x71, 0xC0, 0xC3, 0x07
};
void draw_num_lock(Oled *oled, uint8_t x, uint8_t y) {
  draw_data_arr(oled, x, y, NUM_LOCK_WIDTH, NUM_LOCK_HEIGHT, &num_lock_data[0], sizeof(num_lock_data));
}

void draw_func_indicator(Oled *oled, uint8_t x, uint8_t y) {
  // draws a 9x9 circle
  // Judge me if you want but this is pretty fast
  oled_write_pixel(x+3, y+0, true, oled);
  oled_write_pixel(x+4, y+0, true, oled);
  oled_write_pixel(x+5, y+0, true, oled);

  oled_write_pixel(x+1, y+1, true, oled);
  oled_write_pixel(x+2, y+1, true, oled);
  oled_write_pixel(x+6, y+1, true, oled);
  oled_write_pixel(x+7, y+1, true, oled);

  oled_write_pixel(x+1, y+2, true, oled);
  oled_write_pixel(x+7, y+2, true, oled);

  oled_write_pixel(x, y+3, true, oled);
  oled_write_pixel(x+8, y+3, true, oled);

  oled_write_pixel(x, y+4, true, oled);
  oled_write_pixel(x+8, y+4, true, oled);

  oled_write_pixel(x, y+5, true, oled);
  oled_write_pixel(x+8, y+5, true, oled);

  oled_write_pixel(x+1, y+6, true, oled);
  oled_write_pixel(x+7, y+6, true, oled);

  oled_write_pixel(x+1, y+7, true, oled);
  oled_write_pixel(x+2, y+7, true, oled);
  oled_write_pixel(x+6, y+7, true, oled);
  oled_write_pixel(x+7, y+7, true, oled);

  oled_write_pixel(x+3, y+8, true, oled);
  oled_write_pixel(x+4, y+8, true, oled);
  oled_write_pixel(x+5, y+8, true, oled);
}

void fill_in_func_indicator(Oled *oled, uint8_t x, uint8_t y, bool on) {
  // it fills in the circle made by `draw_func_indicator`
  draw_horizontal_line(oled, x+3, y+1, 3, on);

  draw_horizontal_line(oled, x+2, y+2, 5, on);

  draw_horizontal_line(oled, x+1, y+3, 7, on);

  draw_horizontal_line(oled, x+1, y+4, 7, on);

  draw_horizontal_line(oled, x+1, y+5, 7, on);

  draw_horizontal_line(oled, x+2, y+6, 5, on);

  draw_horizontal_line(oled, x+3, y+7, 3, on);
}


#define GRID_SIZE 14
void draw_switch_grid(Oled *oled, uint8_t x, uint8_t y) {
  //Horizontal lines
  for(uint8_t r = 0; r < MAX_MODULE_ROWS+1; r++) {
    for(uint8_t c = 0; c < 6; c++) {
      draw_horizontal_line(oled, x+c*GRID_SIZE+1, y+r*GRID_SIZE, GRID_SIZE-2, true);
    }
  }

  //Vertical lines
  for(uint8_t c = 0; c < 6+1; c++) {
    for(uint8_t r = 0; r < MAX_MODULE_ROWS; r++) {
      draw_vertical_line(oled, x+c*GRID_SIZE, y+r*GRID_SIZE+1, GRID_SIZE-2, true);
    }
  }
}

void fill_switch_grid_cell(Oled *oled, uint8_t grid_start_x, uint8_t grid_start_y, uint8_t cell_x, uint8_t cell_y, uint8_t level) {
  if(level > 6) { level = 6; }

  uint8_t x = grid_start_x + GRID_SIZE * cell_x;
  uint8_t y = grid_start_y + GRID_SIZE * cell_y;
  uint8_t size = GRID_SIZE/2 - level;

  // Fill what needs to be on
  for(uint8_t row = 0; level > 0 && row < level * 2+1; row++) {
    draw_horizontal_line(oled, x+size, y+row+size, level*2+1, true);
  }

  // Clear the rest:

  //Clear top & bottom
  // for(uint8_t row = 0; row < GRID_SIZE/2 - level; row++) {
  //   draw_horizontal_line(oled, x, y+row, GRID_SIZE, false);
  //   draw_horizontal_line(oled, x, y+GRID_SIZE-row, GRID_SIZE, false);
  // }
  // //Clear the sides
  // for(uint8_t col = 0; col < GRID_SIZE/2 - level; col++) {
  //   draw_vertical_line(oled, x+col, y+GRID_SIZE/2-level, level, false);
  //   draw_vertical_line(oled, x+GRID_SIZE-col, y+GRID_SIZE/2-level, level, false);
  // }
}

#define WPM_CONTAINER_WIDTH 25
#define WPM_CONTAINER_HEIGHT 61

void draw_wpm_container(Oled *oled, uint8_t x, uint8_t y) {
  draw_vertical_line(oled, x+0, y+10, WPM_CONTAINER_HEIGHT-10, true);
  draw_vertical_line(oled, x+1, y+9, WPM_CONTAINER_HEIGHT-9, true);

  draw_vertical_line(oled, x+WPM_CONTAINER_WIDTH, y+10, WPM_CONTAINER_HEIGHT-10, true);
  draw_vertical_line(oled, x+WPM_CONTAINER_WIDTH-1, y+9, WPM_CONTAINER_HEIGHT-9, true);

  draw_horizontal_line(oled, x+2, y+WPM_CONTAINER_HEIGHT-1, WPM_CONTAINER_WIDTH-4, true);
  draw_horizontal_line(oled, x+2, y+WPM_CONTAINER_HEIGHT, WPM_CONTAINER_WIDTH-4, true);


  draw_horizontal_line(oled, x+7, y+WPM_CONTAINER_HEIGHT-2, WPM_CONTAINER_WIDTH-12, true);
  draw_horizontal_line(oled, x+9, y+WPM_CONTAINER_HEIGHT-3, WPM_CONTAINER_WIDTH-16, true);
  draw_horizontal_line(oled, x+11, y+WPM_CONTAINER_HEIGHT-4, WPM_CONTAINER_WIDTH-20, true);
}

void draw_wpm_level_marker(Oled *oled, uint8_t container_x, uint8_t container_y, uint8_t level, bool on) {
  uint8_t x = container_x + 4;
  uint8_t y = container_y + WPM_CONTAINER_HEIGHT - level * 8 - 13;

  // Arrow-y bits
  for(uint8_t col = 0; col < 8; col++) {
    // good luck future sam
    draw_vertical_line(oled, x+col, y + (7-col)/2 + (col+1)%2, 5 + col % 2, on);
    draw_vertical_line(oled, x + WPM_CONTAINER_WIDTH - 8 - col, y + (7-col)/2 + (col+1)%2, 5 + col % 2, on);
  }

  // Center bits
  for(uint8_t col = 0; col < WPM_CONTAINER_WIDTH-23; col++) {
    draw_vertical_line(oled, x+8+col, y, 6, on);
  }
}
