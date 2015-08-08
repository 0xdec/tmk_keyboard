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
// Create an array to store the LED colors
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
        // TODO: change to match the fact that the second chip is shifted by
        // one channel.
        switch (mode) {
            case 0: // Solid color
                TLC.set(i, led_values[group][i].value * brightness);
                break;
            case 1: // Reactive
                if (led_values[group][i].frame < 64) {
                    uint8_t multiplier = pgm_read_byte(animation_table +
                        ((mode - 1) * 64) + led_values[group][i].frame);
                    led_values[group][i].frame++;

                    TLC.set(i, led_values[group][i].value * brightness *
                        multiplier / 255);
                } else if (led_values[group][i].frame == 64) {
                    led_values[group][i].frame = 255;

                    TLC.set((i * 3) + 1, 0);
                    TLC.set(i * 3, 0);
                    TLC.set((i * 3) + 2, 0);
                } else {
                    // TODO: fix this for matrix_row_t and groups
                    if (matrix[group] & (1<<i)) {
                        led_values[group][i].frame = 0;
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
