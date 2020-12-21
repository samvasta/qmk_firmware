#ifndef wing_it_test
#define wing_it_test

#include "quantum.h"
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <LUFA/Drivers/Peripheral/TWI.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "spi.h"
#include "pointing_device.h"

// Helpers
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

// Hardware Pin Defs
#define SPI_OPTION (SPI_SPEED_FCPU_DIV_2 | SPI_SCK_LEAD_FALLING | SPI_SAMPLE_TRAILING | SPI_ORDER_MSB_FIRST | SPI_MODE_MASTER)
#define SENSOR_CS B0
#define SS_TB 0

#define LAYOUT( K1 \
) { \
	{ K1 }  \
}

#endif
