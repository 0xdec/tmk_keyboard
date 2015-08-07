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
#include "lighting.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

// Matrix state (0:off, 1:on)
matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static void init_rows(void);
static uint8_t read_rows(void);
static void init_cols(void);
static void deselect_cols(void);
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

    // Initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;

        // Find the caps and sym lock keys
        // TODO: update when the default layer is changed, and add scroll lock
        for (uint8_t ii = 0; ii < MATRIX_COLS; ii++) {
            // TODO: test default_layer
            uint8_t default_layer = 0;
            uint32_t i = default_layer_state;
            while (i > 1) {
              i = i>>1;
              default_layer++;
            }
            if (keymaps[default_layer][i][ii] == KC_CAPS) {
                caps_key.key.row = i;
                caps_key.key.col = ii;
            } else if (keymaps[default_layer][i][ii] == KC_FN10) {
                sym_key.key.row = i;
                sym_key.key.col = ii;
            }
        }
    }
}

uint8_t matrix_scan(void) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        //_delay_us(3);
        _delay_ms(5);
        uint8_t rows = read_rows();

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            if (DEBOUNCE) {
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
            } else {
                if (rows & _BV(row)) {
                    matrix[row] |= (matrix_row_t)_BV(col);
                } else {
                    matrix[row] &= ~((matrix_row_t)_BV(col));
                }
            }
        }

        deselect_cols();

        // Update the backlight every 5 columns
        if (!(col % 5)) {
            backlight_send_group(col / 5);
        }
    }

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
 * Row: 0    1    2    3    4
 * Pin: PF0  PF4  PF5  PF6  PF7
 */
static void init_rows(void) {
    // Set pins to input (DDR:0, PORT:0)
    DDRF  &= ~0b11110001;
    PORTF &= ~0b11110001;
}

static uint8_t read_rows(void) {
    uint8_t rows = (PINF>>3 & 0b00011110) | (PINF & 1);
    return rows;
}

/* Column pin configuration
 * The columns uses two 74HC238 3 to 8 bit demultiplexers, connected together
 * to form one 4 to 8 bit demultiplexer.
 *
 * The four address lines are named A0, A1, A2, and A3:
 * Name: A0   A1   A2   A3
 * Pin:  PB4  PB5  PB6  PB7
 *
 * There is also an enable pin that must be set correctly:
 * Name: !E1
 * Pin:  PD7
 *
 * To enable both demultiplexers, E1 must be set to 0 (low)
 *
 * Column: 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
 * A0:     0   1   0   1   0   1   0   1   0   1   0   1   0   1   0
 * A1:     0   0   1   1   0   0   1   1   0   0   1   1   0   0   1
 * A2:     0   0   0   0   1   1   1   1   0   0   0   0   1   1   1
 * A3:     0   0   0   0   0   0   0   0   1   1   1   1   1   1   1
 */
static void init_cols(void) {
    // Set pins to output low (DDR:1, PORT:0)
    DDRB |= 0b11110000;
    PORTB &= ~0b11110000;

    DDRD |= _BV(7);
    PORTD &= ~(_BV(7));
}

static void deselect_cols(void) {
    // Set pins to output low (DDR:1, PORT:0)
    PORTB &= ~0b11110000;

    // Set E1 to output high (DDR:1, PORT:1)
    PORTD |= _BV(7);
}

static void select_col(uint8_t col) {
    // Output high (DDR:1, PORT:1) to select
    PORTB |= (col<<4);

    // Set E1 to output low (DDR:1, PORT:0)
    PORTD &= ~(_BV(7));
}
