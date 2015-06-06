#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* 0: Colemak */
    [0] = KEYMAP(
        1   ,2   ,3   ,4   ,5   ,6   ,LGUI,ESC ,RGUI,7   ,8   ,9   ,0   ,MINS,EQL ,
        GRV ,Q   ,W   ,F   ,P   ,G   ,CAPS,UP  ,NO  ,J   ,L   ,U   ,Y   ,LBRC,RBRC,
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
        GRV ,QUOT,COMM,DOT ,P   ,Y   ,CAPS,UP  ,NO  ,F   ,G   ,C   ,R   ,L   ,SLSH,
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
        GRV ,Q   ,W   ,E   ,R   ,T   ,CAPS,UP  ,NO  ,Y   ,U   ,I   ,O   ,P   ,SLSH,
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
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,SLEP,TRNS,MPRV,MPLY,MNXT,MUTE,VOLD,VOLU,
        NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,TRNS,TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,
        NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,TRNS,TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,
        NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,TRNS,TRNS,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,
        TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS   ,TRNS  ,TRNS  ),
};
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_DEFAULT_LAYER_SET(0),  // set colemak layout
    [1] = ACTION_LAYER_MOMENTARY(1),  // to function layer

    [2] = ACTION_DEFAULT_LAYER_SET(2),  // set dvorak layout
    [3] = ACTION_LAYER_MOMENTARY(3),  // to function layer

    [4] = ACTION_DEFAULT_LAYER_SET(4),  // set qwerty layout
    [5] = ACTION_LAYER_MOMENTARY(5),  // to function layer

    [6] = ACTION_LAYER_MOMENTARY(6),  // to media layer
    [7] = ACTION_BACKLIGHT_TOGGLE(),
    [8] = ACTION_BACKLIGHT_DECREASE(),
    [9] = ACTION_BACKLIGHT_INCREASE(),
};
