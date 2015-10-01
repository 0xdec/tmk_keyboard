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

#include "keymap_common.h"
//#include "bootloader.h"


/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
    return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode) {
    /*if (keycode == KC_SYM) {
        return (action_t){ .code = ACTION_LAYER_TOGGLE(7) };
    }*/

    return (action_t){ .code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]) };
}
