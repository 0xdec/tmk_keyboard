ION-60 Keyboard Firmware
======================
DIY/Assembled ortholinear 60% keyboard by [D1SC0tech](http://d1sc0te.ch).

## Build

Follow [this guide](http://deskthority.net/workshop-f7/how-to-build-your-very-own-keyboard-firmware-t7177.html) to setup your development environment before anything else. Abbreviated instructions are provide at the [bottom of this document](https://github.com/rswiernik/tmk_keyboard/tree/rswiernik_dev/keyboard/planck#environment-setup)

Download the whole firmware [here](https://github.com/jackhumbert/tmk_keyboard/archive/master.zip) and navigate to the keyboard/planck folder. Once your dev env is setup, you'll be able to type `make` to generate your .hex that you can load with the Teensy app onto your Planck (once you've hit reset/shorted GND & RST).

Depending on which keymap you would like to use, you will have to compile slightly differently.

####Default
To build with the default keymap, simply move to the tmk\_keyboard/keyboard/ion60/ and run `make` as follows:
```
$ make
```

## Keymap
Several version of keymap are available in advance but you are recommended to define your favorite layout yourself. To define your own keymap create file named `keymap_<name>.c` and see keymap document (you can find in top README.md) and existent keymap files.

####**Extended Keymaps**

To build the firmware binary hex file with an extended keymap just do `make` with `KEYMAP` option like:
```
$ make KEYMAP=[common|jack|<name>]
```
_The only applicable keymaps will work with this option._ Extended keymaps follow the format **__extended\_keymap\_\<name\>.c__**

####**Common Keymaps**

Building with a common keymap is as simple as adding the COMMON option. Note that only
```
$ make KEYMAP=[common|jack|<name>] COMMON=true
```
_The only applicable keymaps will work with this option._ Common keymaps follow the format **__keymap\_\<name\>.c__**
