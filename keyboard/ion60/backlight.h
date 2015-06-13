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

#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#define NUM_MODES 1

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    uint8_t frame;
} rgbled_t

void backlight_send_row(uint8_t row);

uint8_t backlight_mode_get(void);
void backlight_mode_set(uint8_t mode_new);
void backlight_mode_next(void);
void backlight_mode_prev(void);

void backlight_caps_lock(bool state);
//void backlight_scroll_lock(bool state);

#endif // BACKLIGHT_H
