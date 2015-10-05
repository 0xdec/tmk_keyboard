/*
Copyright 2015 Jordi Pakey-Rodriguez <jordi.orlando@d1sc0te.ch>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//#include "led.h"
#include "lighting.h"
#include <avr/pgmspace.h> // Include the pgmspace library for PROGMEM support
#include "TLC5947/TLC5947.h"

// Declare TLC5947 devices
// TODO: change BLANK pin
TLC5947 TLC(PB0, PD7);

//extern matrix_row_t matrix[];

static const uint8_t animation_table[NUM_MODES][64] PROGMEM = {
    // Mode: Reactive (quick fade on strike)
    // Wolfram Alpha:
    // Table[floor(Re(ln(x))*255/Re(ln(0.015625))), {x, 0.015625, 1, 0.015625}]
    {
        255,212,187,170,156,145,135,127,120,113,107,102,97 ,93 ,88 ,85 ,
        81 ,77 ,74 ,71 ,68 ,65 ,62 ,60 ,57 ,55 ,52 ,50 ,48 ,46 ,44 ,42 ,
        40 ,38 ,37 ,35 ,33 ,31 ,30 ,28 ,27 ,25 ,24 ,22 ,21 ,20 ,18 ,17 ,
        16 ,15 ,13 ,12 ,11 ,10 ,9  ,8  ,7  ,6  ,4  ,3  ,2  ,1  ,0  ,0
    }
};
/*static const uint8_t led_map[MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    {3, 7, 11, 15, 19, 23, 39, 43, 47, 71, 67, 63, 59, 55, 51},
    {2, 6, 10, 14, 18, 22, 38, 42, 46, 70, 66, 62, 58, 54, 50},
    {1, 5, 9, 13, 17, 21, 37, 41, 45, 69, 65, 61, 57, 53, 49},
    {0, 4, 8, 12, 16, 20, 36, 40, 44, 68, 64, 60, 56, 52, 48},
    {25, 24, 26, 27, 24, 28, 29, 255, 30, 31, 35, 32, 33, 35, 34}
};*/
// Array to map the LED groups to the switch_leds array
static const uint8_t group_map[NUM_GROUPS][24] PROGMEM = {
    {0x30,0x20,0x10,0x00,0x31,0x21,0x11,0x01,0x32,0x22,0x12,0x02,0x33,0x23,0x13,0x03,0x34,0x24,0x14,0x04,0x35,0x25,0x15,0x05},
    {0x3E,0x2E,0x1E,0x0E,0x3D,0x2D,0x1D,0x0D,0x3C,0x2C,0x1C,0x0C,0x3B,0x2B,0x1B,0x0B,0x3A,0x2A,0x1A,0x0A,0x39,0x29,0x19,0x09},
    {0x40,0x42,0x43,0x45,0x46,0x48,0x49,0x4B,0x4C,0x4E,0xF0,0xF1,0x36,0x26,0x16,0x06,0x37,0x27,0x17,0x07,0x38,0x28,0x18,0x08},
    {0x41,0x44,0x4A,0x4D,0xF2,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0xB0,0xB1,0xB2}
};
// Array for the group anode pins
static const pin group_pins[NUM_GROUPS] = {
    PB4, PB5, PB6, PB7
}
// Arrays to store the LED values
static led_t switch_leds[MATRIX_ROWS][MATRIX_COLS];
static led_t lock_leds[3];
static led_t blue_leds[3];
static led_t batt_leds[16];

// Global keyboard brightness, goes from 0(off) to 16(full on)
extern uint8_t brightness;
// Animation mode. 0 is solid, 1 is reactive, etc.
uint8_t mode = 0;
// Lock keys
//keyevent_t caps_key, sym_key;



/*void backlight_init() {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        for (uint8_t ii = 0; ii < MATRIX_COLS; ii++) {
            switch_leds[i][ii].value = 255;
            switch_leds[i][ii].frame = 255;
        }
    }
}*/

void backlight_send_group(uint8_t group) {
    // TODO: needs code for LED anodes (rows)

    for (uint8_t i = 0; i < 24; i++) {
        uint8_t led_col = pgm_read_byte(&group_map[group][i]);

        if ((col & 0xF0) == 0xF0) {
            // Lock key LED
            TLC.set(i, lock_leds[col & 0x0F].value * brightness);
        } else if ((col & 0xF0) == 0xB0) {
            // Bluetooth indicator LED
            TLC.set(i, blue_leds[col & 0x0F].value * brightness);
        } else if ((col & 0xF0) == 0x80) {
            // Battery indicator LED
            TLC.set(i, batt_leds[col & 0x0F].value * brightness);
        } else {
            uint8_t led_row = (led_col >> 4) & 0x07;
            led_col &= 0x0F;

            switch (mode) {
                case 0: // Solid color
                    TLC.set(i, switch_leds[led_row][led_col].value * brightness);
                    break;
                case 1: // Reactive
                    if (switch_leds[led_row][led_col].frame < 64) {
                        uint8_t multiplier = pgm_read_byte(animation_table +
                            ((mode - 1) * 64) + switch_leds[led_row][led_col].frame);
                        switch_leds[led_row][led_col].frame++;

                        TLC.set(i, switch_leds[led_row][led_col].value * brightness *
                            multiplier / 255);
                    } else if (switch_leds[led_row][led_col].frame == 64) {
                        switch_leds[led_row][led_col].frame = 255;

                        TLC.set((i * 3) + 1, 0);
                        TLC.set(i * 3, 0);
                        TLC.set((i * 3) + 2, 0);
                    } else {
                        // TODO: fix this for matrix_row_t and groups
                        if (matrix[led_row] & (1<<i)) {
                            switch_leds[led_row][led_col].frame = 0;
                        }
                    }
                    break;
            }
        }
    }

    // Send the data to the chips
    TLC5947::update();
}

uint8_t backlight_mode_get(void) {
    return mode;
}

void backlight_mode_set(uint8_t mode_new) {
    mode = mode_new;
}

void backlight_mode_next(void) {
    mode++;
    if (mode > NUM_MODES) {
        mode = 0;
    }
}

void backlight_mode_prev(void) {
    mode--;
    if (mode > NUM_MODES) {
        mode = NUM_MODES;
    }
}

void backlight_caps_lock(bool state) {
    caps_key.pressed = state;

    if (state) {
        lock_leds[0].value = 255;
    } else {
        lock_leds[0].value = 0;
    }
}

/*void backlight_scroll_lock(bool state) {
    scroll_key.pressed = state;

    if (state) {
        lock_leds[2].value = 255;
    } else {
        lock_leds[2].value = 0;
    }
}*/
