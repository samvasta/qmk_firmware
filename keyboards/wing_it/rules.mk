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
BOOTLOADER = halfkay # atmel-dfu


# Build Options
#   comment out to disable the options.
#
CUSTOM_MATRIX           = yes
UNICODE_ENABLE          = no
BOOTMAGIC_ENABLE        = no
MOUSEKEY_ENABLE         = no
EXTRAKEY_ENABLE         = yes # Audio control and System control
CONSOLE_ENABLE          = no # Console for debug
COMMAND_ENABLE          = no
NKRO_ENABLE             = yes
USB_6KRO_ENABLE         = no
BACKLIGHT_ENABLE        = no
SWAP_HANDS_ENABLE       = no
RGBLIGHT_ENABLE         = no
MIDI_ENABLE             = no
OLED_DRIVER_ENABLE      = yes
WPM_ENABLE              = yes


# project specific files
SRC = matrix.c \
			module.c \
			i2c_master.c
	    # twimaster.c \

LAYOUTS = default
