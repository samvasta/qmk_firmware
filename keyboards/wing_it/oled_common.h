#pragma once
#include "oled_driver.h"

void oled_pre_task(void);
void oled_task_left(Oled *oled);
void oled_task_right(Oled *oled);

void draw_horizontal_line(Oled *oled, uint8_t x, uint8_t y, uint8_t len, bool on);
void draw_vertical_line(Oled *oled, uint8_t x, uint8_t y, uint8_t len, bool on);
void draw_data_arr(Oled *oled, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *data, uint8_t data_len);

void draw_lock_open(Oled *oled, uint8_t x, uint8_t y);
void draw_lock_closed(Oled *oled, uint8_t x, uint8_t y);
void draw_caps_lock(Oled *oled, uint8_t x, uint8_t y);
void draw_num_lock(Oled *oled, uint8_t x, uint8_t y);

void draw_func_indicator(Oled *oled, uint8_t x, uint8_t y);
void fill_in_func_indicator(Oled *oled, uint8_t x, uint8_t y, bool on);

void draw_switch_grid(Oled *oled, uint8_t x, uint8_t y);
void fill_switch_grid_cell(Oled *oled, uint8_t grid_start_x, uint8_t grid_start_y, uint8_t cell_x, uint8_t cell_y, uint8_t level);

void draw_wpm_container(Oled *oled, uint8_t x, uint8_t y);
void draw_wpm_level_marker(Oled *oled, uint8_t container_x, uint8_t container_y, uint8_t level, bool on);
