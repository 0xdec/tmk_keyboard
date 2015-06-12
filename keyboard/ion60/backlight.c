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
#include "backlight.h"
#include "TLC5947.h"

TLC5947 TLC1;
TLC5947 TLC2;

// Create an array to store the LED values
uint16_t anChannels[48];

//void backlight_init() {
//}

void backlight_set(uint8_t level) {

}

void set_caps_lock(bool state) {

}

void set_scroll_lock(bool state) {

}
