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

#ifndef CONFIG_H
#define CONFIG_H


/* USB Device descriptor parameters */
// TODO: look into getting an Openmoko VID/PID
// http://wiki.openmoko.org/wiki/USB_Product_IDs
#define VENDOR_ID       0xFEED    // Openmoko: 0x1D50
#define PRODUCT_ID      0x1060    // 10 is for ION, 60 is for 60
#define DEVICE_VER      0x0100    // BCD notation for v1.0.0
#define MANUFACTURER    D1SC0tech // Dat me!
#define PRODUCT         ION-60    // ION-KB 60% keyboard
#define DESCRIPTION     0x000     // This is the serial number, not the description

/* Key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15

/* Number of backlight levels */
#define BACKLIGHT_LEVELS 16

/* Define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 0

/* Key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG // TODO: uncomment for production

/* disable print */
//#define NO_PRINT // TODO: uncomment for production

/* disable action features */
//#define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION


#endif
