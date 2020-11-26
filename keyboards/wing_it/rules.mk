MCU = atmega32u4
F_CPU = 16000000
ARCH = AVR8
F_USB = $(F_CPU)
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = atmel-dfu


# Build Options
#   comment out to disable the options.
#
CUSTOM_MATRIX           = yes
UNICODE_ENABLE          = yes
BOOTMAGIC_ENABLE        = no
MOUSEKEY_ENABLE         = yes
EXTRAKEY_ENABLE         = no # Audio control and System control
CONSOLE_ENABLE          = yes # Console for debug
COMMAND_ENABLE          = no
NKRO_ENABLE             = yes
USB_6KRO_ENABLE         = no
BACKLIGHT_ENABLE        = no
SWAP_HANDS_ENABLE       = no
RGBLIGHT_ENABLE         = no
MIDI_ENABLE             = no
OLED_DRIVER_ENABLE      = yes


# project specific files
SRC = matrix.c \
	    i2c_master.c \
			module.c

LAYOUTS = default
