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
TLC5947 TLC(PB3, PB0);

//extern matrix_row_t matrix[];

static const uint8_t animation_table[NUM_MODES][64] PROGMEM = {
    // Mode: Reactive (quick fade on strike)
    // Wolfram Alpha:
    // Table[floor(Re(ln(x))*255/Re(ln(0.015625))), {x, 0.015625, 1, 0.015625}]
    {
        255,212,187,170,156,145,135,127,120,113,107,102,97,93,88,85,
        81,77,74,71,68,65,62,60,57,55,52,50,48,46,44,42,
        40,38,37,35,33,31,30,28,27,25,24,22,21,20,18,17,
        16,15,13,12,11,10,9,8,7,6,4,3,2,1,0,0
    }
};
/*static const uint8_t led_map[MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    {3, 7, 11, 15, 19, 23, 39, 43, 47, 71, 67, 63, 59, 55, 51},
    {2, 6, 10, 14, 18, 22, 38, 42, 46, 70, 66, 62, 58, 54, 50},
    {1, 5, 9, 13, 17, 21, 37, 41, 45, 69, 65, 61, 57, 53, 49},
    {0, 4, 8, 12, 16, 20, 36, 40, 44, 68, 64, 60, 56, 52, 48},
    {25, 24, 26, 27, 24, 28, 29, 255, 30, 31, 35, 32, 33, 35, 34}
};*/
// Array to map the LED groups to the value array
// TODO: check LEDs 61 & 73 (64 & 70 are being ignored)
static const uint8_t group_map[NUM_GROUPS][24] PROGMEM = {
    {45,30,15,0 ,46,31,16,1 ,47,32,17,2 ,48,33,18,3 ,49,34,19,4 ,50,35,20,5},
    {61,60,62,63,65,66,68,69,71,72,74,73,51,36,21,6 ,52,37,22,7 ,53,38,23,8},
    {59,44,29,14,58,43,28,13,57,42,27,12,56,41,26,11,55,40,25,10,54,39,24,9}
};
// Array for the group anode pins
static const pin_t group_pins[NUM_GROUPS] = {
    PB5,
    PB6,
    PB7
};
// Array to store the LED values
static led_t led_values[MATRIX_ROWS][MATRIX_COLS];

// Global keyboard brightness, goes from 0(off) to 16(full on)
extern uint8_t brightness;
// Animation mode. 0 is solid, 1 is reactive, etc.
uint8_t mode = 0;
// Lock keys
//keyevent_t caps_key, sym_key;



/*void backlight_init() {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        for (uint8_t ii = 0; ii < MATRIX_COLS; ii++) {
            led_values[i][ii].value = 255;
            led_values[i][ii].frame = 255;
        }
    }
}*/

void backlight_send_group(uint8_t group) {
    // TODO: needs code for LED anodes (rows)

    for (uint8_t i = 0; i < 24; i++) {
        uint8_t led_row = pgm_read_byte(&group_map[group][i]);
        uint8_t led_col = led_row % 24;
        led_row = (led_row - led_col) / 24;

        switch (mode) {
            case 0: // Solid color
                TLC.set(i, led_values[led_row][led_col].value * brightness);
                break;
            case 1: // Reactive
                if (led_values[led_row][led_col].frame < 64) {
                    uint8_t multiplier = pgm_read_byte(animation_table +
                        ((mode - 1) * 64) + led_values[led_row][led_col].frame);
                    led_values[led_row][led_col].frame++;

                    TLC.set(i, led_values[led_row][led_col].value * brightness *
                        multiplier / 255);
                } else if (led_values[led_row][led_col].frame == 64) {
                    led_values[led_row][led_col].frame = 255;

                    TLC.set((i * 3) + 1, 0);
                    TLC.set(i * 3, 0);
                    TLC.set((i * 3) + 2, 0);
                } else {
                    // TODO: fix this for matrix_row_t and groups
                    if (matrix[led_row] & (1<<i)) {
                        led_values[led_row][led_col].frame = 0;
                    }
                }
                break;
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
    led_t temp;

    if (state) {
        temp.value = 255;
    } else {
        temp.value = 0;
    }

    led_values[caps_key.key.row][caps_key.key.col] = temp;
}

/*void backlight_scroll_lock(bool state) {
    scroll_key.pressed = state;
}*/
