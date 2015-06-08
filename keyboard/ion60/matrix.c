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


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

// Matrix state (0:off, 1:on)
static matrix_row_t matrix[MATRIX_ROWS];
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
    }
}

uint8_t matrix_scan(void) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        _delay_us(3);
        uint8_t rows = read_rows();

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)1<<col);
            bool curr_bit = rows & (1<<row);

            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)1<<col);
                if (debouncing) {
                    dprint("bounce!: ");
                    dprintf("%02X", debouncing);
                    dprintln();
                }
                debouncing = DEBOUNCE;
            }
        }

        deselect_cols();
    }

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
    return (matrix[row] & ((matrix_row_t)1<<col));
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
 * Pin: PD0  PD1  PD2  PD3  PD5
 */
static void init_rows(void) {
    // Set pins to input (DDR:0, PORT:0)
    DDRD  &= ~0b00101111;
    PORTD &= ~0b00101111;
}

static uint8_t read_rows(void) {
    return  (PIND & 0b00001111) |
            (PIND & (1<<5) ? (1<<4) : 0);
}

/* Column pin configuration
 * The columns uses two 74HC237D 3 to 8 bit demultiplexers, connected together
 * to form one 4 to 8 bit demultiplexer.
 *
 * The four address lines are named A0, A1, A2, and A3:
 * Name: A0   A1   A2   A3
 * Pin:  PF4  PF5  PF6  PF7
 *
 * There are also two enable pins that must be set correctly:
 * Name: E0   E1
 * Pin:  PF0  PF1
 *
 * To enable both demultiplexers, E0 must be set to 1 (high) and E1 to 0 (low)
 *
 * Column: 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14
 * A0:     0   1   0   1   0   1   0   1   0   1   0   1   0   1   0
 * A1:     0   0   1   1   0   0   1   1   0   0   1   1   0   0   1
 * A2:     0   0   0   0   1   1   1   1   0   0   0   0   1   1   1
 * A3:     0   0   0   0   0   0   0   0   1   1   1   1   1   1   1
 */
static void init_cols(void) {
    // Set pins to output low (DDR:1, PORT:0)
    DDRF |= 0b11110011;
    PORTF &= ~0b11110011;
}

static void deselect_cols(void) {
    // Set E0 to output low (DDR:1, PORT:0)
    PORTF &= ~1;
}

static void select_col(uint8_t col) {
    // Output high (DDR:1, PORT:1) to select
    PORTF |= 1 | (col<<4);
}
