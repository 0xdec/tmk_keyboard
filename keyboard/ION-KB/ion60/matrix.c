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

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "keymap_common.h"
#include "keycode.h"
#include "TLC5947/pindefs.h"

#ifndef NO_BACKLIGHT
#include "lighting.h"
#endif


const pin_t SIN = PC6;
const pin_t CLK = PC7;
const pin_t LAT = PE6;

#ifdef SOFT_DEBOUNCE
#   define DEBOUNCE	5
#else
#   define DEBOUNCE 0
#endif
static uint8_t debouncing = DEBOUNCE;

// Matrix state (0:off, 1:on)
matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static void init_rows(void);
static uint8_t read_rows(void);
static void init_cols(void);
static void deselect_cols(uint8_t col);
static void select_col(uint8_t col);


inline
uint8_t matrix_rows(void) {
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void) {
    return MATRIX_COLS;
}

void matrix_init(void) {
    // Initialize rows and columns
    init_rows();
    init_cols();

    // TODO: test default_layer
    uint8_t default_layer = 0;
    uint32_t i = default_layer_state;
    while (i > 1) {
      i = i>>1;
      default_layer++;
    }

    // Initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;

        #ifndef NO_BACKLIGHT
        // Find the caps and sym lock keys
        // TODO: update when the default layer is changed, and add scroll lock
        for (uint8_t ii = 0; ii < MATRIX_COLS; ii++) {
            uint8_t key = pgm_read_byte(&keymaps[default_layer][i][ii]);
            if (key == KC_CAPS) {
                caps_key.key.row = i;
                caps_key.key.col = ii;
            } else if (key == KC_FN10) {
                sym_key.key.row = i;
                sym_key.key.col = ii;
            }
        }
        #endif
    }
}

uint8_t matrix_scan(void) {
    // Set SIN to output high (DDR:1, PORT:1)
    *SIN.port |= _BV(SIN.pin);

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);

        // Set SIN to output low (DDR:1, PORT:0)
        *SIN.port &= ~(_BV(SIN.pin));

        #ifdef SOFT_DEBOUNCE
        _delay_us(3);
        #else
        _delay_ms(5);
        #endif

        uint8_t rows = read_rows();

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            #ifdef SOFT_DEBOUNCE
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)_BV(col));
            bool curr_bit = rows & _BV(row);

            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)_BV(col));
                if (debouncing) {
                    dprint("bounce!: ");
                    dprintf("%02X", debouncing);
                    dprintln();
                }
                debouncing = DEBOUNCE;
            }
            #else
            if (rows & _BV(row)) {
                matrix[row] |= (matrix_row_t)_BV(col);
            } else {
                matrix[row] &= ~((matrix_row_t)_BV(col));
            }
            #endif
        }

        #ifndef NO_BACKLIGHT
        // Update the backlight every 4 columns
        // TODO: make this even (figure out time for each cycle)
        if (!(col % 4)) {
            backlight_send_group(col / 4);
        }
        #endif
    }

    deselect_cols(MATRIX_COLS);

    matrix_print();

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void) {
    if (debouncing) return false;
    else return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & ((matrix_row_t)_BV(col)));
}

inline
matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

void matrix_print(void) {
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        print_hex8(row);
        print(": ");
        print_bin_reverse32(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop32(matrix[i]);
    }
    return count;
}

/* Row pin configuration
 * Row: 0   1   2   3   4       5 (optional)
 * Pin: PF0 PF1 PF4 PF5 PF6     PF7
 */
static void init_rows(void) {
    // Set pins to input (DDR:0, PORT:0)
    DDRF  &= ~0b01110011;
    PORTF &= ~0b01110011;

    if (MATRIX_ROWS == 6) {
        DDRF  &= ~0b10000000;
        PORTF &= ~0b10000000;
    }
}

static uint8_t read_rows(void) {
    uint8_t rows = (PINF>>2 & 0b00011100) | (PINF & 0b00000011);
    if (MATRIX_ROWS == 6) {
        rows |= (PINF>>7 & 1);
    }
    return rows;
}

/* Column pin configuration
 * The columns use two 74HC595 8 bit serial->parallel shift registers, connected
 * together to form one 16 bit serial->parallel shift register.
 *
 * The control lines are SIN, CLK, and LAT:
 * Name:  SIN CLK LAT
 * Pin:   PC6 PC7 PE6
 *
 * To enable both shift registers, !OE must be tied to ground
 */
static void init_cols(void) {
    // Set control pins to output low (DDR:1, PORT:0)
    *SIN.ddr |= _BV(SIN.pin);
    *CLK.ddr |= _BV(CLK.pin);
    *LAT.ddr |= _BV(LAT.pin);

    *SIN.port &= ~(_BV(SIN.pin));
    *CLK.port &= ~(_BV(CLK.pin));
    *LAT.port &= ~(_BV(LAT.pin));

    deselect_cols(0);
}

static void deselect_cols(uint8_t col) {
    // Set LAT to output low (DDR:1, PORT:0)
    *LAT.port &= ~(_BV(LAT.pin));
    // Set SIN to output low (DDR:1, PORT:0)
    *SIN.port &= ~(_BV(SIN.pin));

    for (uint8_t i = 0; i < (16 - col); i++) {
        // Toggle CLK on and off
        *CLK.port |= _BV(CLK.pin);
        *CLK.port &= ~(_BV(CLK.pin));
    }

    // Set LAT to output high (DDR:1, PORT:1)
    *LAT.port |= _BV(LAT.pin);
}

static void select_col(uint8_t col) {
    // Set LAT to output low (DDR:1, PORT:0)
    *LAT.port &= ~(_BV(LAT.pin));

    // Toggle CLK on and off
    *CLK.port |= _BV(CLK.pin);
    *CLK.port &= ~(_BV(CLK.pin));

    // Set LAT to output high (DDR:1, PORT:1)
    *LAT.port |= _BV(LAT.pin);
}
