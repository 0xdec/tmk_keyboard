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

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: Colemak */
    [0] = KEYMAP(
        1   ,2   ,3   ,4   ,5   ,6   ,LGUI,ESC ,RGUI,7   ,8   ,9   ,0   ,MINS,EQL ,
        GRV ,Q   ,W   ,F   ,P   ,G   ,CAPS,UP  ,FN10,J   ,L   ,U   ,Y   ,LBRC,RBRC,
        TAB ,A   ,R   ,S   ,T   ,D   ,LEFT,DOWN,RGHT,H   ,N   ,E   ,I   ,O   ,QUOT,
        BSLS,Z   ,X   ,C   ,V   ,B   ,FN1 ,FN6 ,FN1 ,K   ,M   ,COMM,DOT ,SLSH,SCLN,
        LALT  ,LCTL   ,LSFT  ,BSPC   ,DEL   ,ENT    ,SPC   ,RSFT   ,RCTL  ,RALT  ),
    /* 1: Function */
    [1] = KEYMAP(
        F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,TRNS,TRNS,TRNS,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,
        TRNS,FN4 ,TRNS,TRNS,PSCR,TRNS,TRNS,PGUP,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,SLCK,TRNS,FN2 ,HOME,PGDN,END ,TRNS,TRNS,TRNS,INS ,TRNS,TRNS,
        TRNS,TRNS,TRNS,FN0 ,TRNS,PAUS,TRNS,TRNS,TRNS,TRNS,MENU,TRNS,TRNS,TRNS,TRNS,
        TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS  ),

    /* 2: Dvorak */
    [2] = KEYMAP(
        1   ,2   ,3   ,4   ,5   ,6   ,LGUI,ESC ,RGUI,7   ,8   ,9   ,0   ,LBRC,RBRC,
        GRV ,QUOT,COMM,DOT ,P   ,Y   ,CAPS,UP  ,FN10,F   ,G   ,C   ,R   ,L   ,SLSH,
        TAB ,A   ,O   ,E   ,U   ,I   ,LEFT,DOWN,RGHT,D   ,H   ,T   ,N   ,S   ,MINS,
        BSLS,SCLN,Q   ,J   ,K   ,X   ,FN3 ,FN6 ,FN3 ,B   ,M   ,W   ,V   ,Z   ,EQL ,
        LALT  ,LCTL   ,LSFT  ,BSPC   ,DEL   ,ENT    ,SPC   ,RSFT   ,RCTL  ,RALT  ),
    /* 3: Function */
    [3] = KEYMAP(
        F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,TRNS,TRNS,TRNS,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,
        TRNS,TRNS,TRNS,TRNS,PSCR,TRNS,TRNS,PGUP,TRNS,TRNS,TRNS,FN0 ,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,INS ,HOME,PGDN,END ,FN2 ,TRNS,TRNS,TRNS,SLCK,TRNS,
        TRNS,TRNS,FN4 ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,PAUS,MENU,TRNS,TRNS,TRNS,TRNS,
        TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS  ),

    /* 4: QWERTY */
    [4] = KEYMAP(
        1   ,2   ,3   ,4   ,5   ,6   ,LGUI,ESC ,RGUI,7   ,8   ,9   ,0   ,MINS,EQL ,
        GRV ,Q   ,W   ,E   ,R   ,T   ,CAPS,UP  ,FN10,Y   ,U   ,I   ,O   ,P   ,SLSH,
        TAB ,A   ,S   ,D   ,F   ,G   ,LEFT,DOWN,RGHT,H   ,J   ,K   ,L   ,SCLN,QUOT,
        BSLS,Z   ,X   ,C   ,V   ,B   ,FN5 ,FN6 ,FN5 ,N   ,M   ,COMM,DOT ,LBRC,RBRC,
        LALT  ,LCTL   ,LSFT  ,BSPC   ,DEL   ,ENT    ,SPC   ,RSFT   ,RCTL  ,RALT  ),
    /* 5: Function */
    [5] = KEYMAP(
        F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,TRNS,TRNS,TRNS,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,
        TRNS,FN4 ,TRNS,TRNS,TRNS,TRNS,TRNS,PGUP,TRNS,TRNS,TRNS,INS ,TRNS,PSCR,TRNS,
        TRNS,TRNS,SLCK,FN2 ,TRNS,TRNS,HOME,PGDN,END ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,FN0 ,TRNS,PAUS,TRNS,TRNS,TRNS,TRNS,MENU,TRNS,TRNS,TRNS,TRNS,
        TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS  ),

    /* 6: Media */
    [6] = KEYMAP(
        NO  ,NO  ,NO  ,FN7 ,FN8 ,FN9 ,TRNS,SLEP,TRNS,MPRV,MPLY,MNXT,MUTE,VOLD,VOLU,
        NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,WREF,TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,
        NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,WBAK,WSTP,WFWD,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,
        NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,TRNS,TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,
        TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS  ),

    /* 10: Shift lock */
    [10] = KEYMAP(
        FN11,FN11,FN11,FN11,FN11,FN11,TRNS,TRNS,TRNS,FN11,FN11,FN11,FN11,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS  ),
};
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_DEFAULT_LAYER_SET(0), // Set colemak as default layout
    [1] = ACTION_LAYER_MOMENTARY(1), // To function layer

    [2] = ACTION_DEFAULT_LAYER_SET(2), // Set dvorak as default layout
    [3] = ACTION_LAYER_MOMENTARY(3), // To function layer

    [4] = ACTION_DEFAULT_LAYER_SET(4), // Set qwerty as default layout
    [5] = ACTION_LAYER_MOMENTARY(5), // To function layer

    [6] = ACTION_LAYER_MOMENTARY(6), // To media layer
    [7] = ACTION_BACKLIGHT_TOGGLE(),
    [8] = ACTION_BACKLIGHT_DECREASE(),
    [9] = ACTION_BACKLIGHT_INCREASE(),

    [10] = ACTION_LAYER_TOGGLE(10), // Shift lock
    [11] = ACTION_FUNCTION(SHIFT_LOCK),
};

/*
 * user defined action function
 */
enum function_id {
    SHIFT_LOCK,
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
    // TODO: move these inside the case?
    static uint8_t mods_shift;
    static uint8_t number_key;

    switch (id) {
        case SHIFT_LOCK:
            mods_shift = get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));
            number_key = keymap_key_to_keycode(default_layer, record->event.key);
            if (record->event.pressed) {
                if (mods_shift) {
                    del_mods(mods_shift); // Remove shift
                    add_key(number_key); // Add the number
                    send_keyboard_report(); // Send a keydown report
                    add_mods(mods_shift); // Add shift
                } else {
                    add_mods(mods_shift); // Add shift
                    add_key(number_key); // Add the number
                    send_keyboard_report(); // Send a keydown report
                    del_mods(mods_shift); // Remove shift
                }
            } else {
                del_key(number_key); // Remove the number on keyup
                send_keyboard_report(); // Send a keyup report
            }
            break;
    }
}
