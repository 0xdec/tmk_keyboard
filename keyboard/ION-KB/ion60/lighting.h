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

#ifndef LIGHTING_H
#define LIGHTING_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

//#include <stdint.h>
//#include <stdbool.h>
#include "keyboard.h" // For keyevent_t
#include "matrix.h" // For matrix_row_t

#define NUM_MODES 1

typedef struct {
    uint8_t value;
    uint8_t frame;
} led_t;

extern matrix_row_t matrix[];

keyevent_t caps_key, sym_key;

EXTERNC void backlight_send_group(uint8_t group);

uint8_t backlight_mode_get(void);
void backlight_mode_set(uint8_t mode_new);
void backlight_mode_next(void);
void backlight_mode_prev(void);

EXTERNC void backlight_caps_lock(bool state);
//void backlight_scroll_lock(bool state);

#undef EXTERNC

#endif // LIGHTING_H
