#include "wing_it_test.h"

volatile int8_t pointer_dx = 90;
volatile int8_t pointer_dy = -78;

// Trackball State
bool BurstState = false;  // init burst state for Trackball module
float mouse_multiplier = 1.0;

// Mouse Processing
static void process_mouse(void) {
    // Read state
    PMWState d = point_burst_read(Motion, &BurstState);
    int16_t x, y;

    pointer_dx = (int)d.X;
    pointer_dy = (int)d.Y;

    if (d.X == 0 && d.Y == 0) {
        return;
    }

    // apply multiplier
    x *= mouse_multiplier;
    y *= mouse_multiplier;

    // Wrap to HID size
    x = constrain(d.X, -127, 127);
    y = constrain(d.Y, -127, 127);

    report_mouse_t currentReport = pointing_device_get_report();
    currentReport.x = (int)x;
    currentReport.y = (int)y;

    pointing_device_set_report(currentReport);
    pointing_device_send();
}



// Hardware Setup
// void keyboard_pre_init_kb(void) {
void pointing_device_init(void) {
    // Set up all the hardware
    setPinOutput(SENSOR_CS);

    // Initialize SPI for MCU
    SPI_Init(SPI_OPTION);
    DDRB |= (1 << SS_TB);
    PORTB |= (1 << SS_TB);  // pull up to diable all comm
    point_init(SS_TB);
}

void pointing_device_task(void) {
    process_mouse();
}
