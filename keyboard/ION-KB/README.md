ION-KB Keyboard Firmware
======================
Fancy ortholinear keyboards by [D1SC0tech](http://d1sc0te.ch).

## Build

Follow [this guide](http://deskthority.net/workshop-f7/how-to-build-your-very-own-keyboard-firmware-t7177.html) to setup your development environment before anything else.

Download the firmware [here](https://github.com/D1SC0tech/tmk_keyboard/archive/master.zip) and navigate to the keyboard/ION-KB/ionXX folder, where 'XX' is the size of your keyboard (20, 40, or 60). Once your dev env is setup, you'll be able to type `make` to generate your .hex file that you can load with dfu-programmer onto your keyboard (once you've pressed the reset button).

Depending on which keymap you would like to use, you will have to compile slightly differently.

####Default
To build with the default keymap, simply cd to the tmk\_keyboard/keyboard/ION-KB/ionXX directory and run the `make` command:
```
$ make
```

## Keymap
Several pre-made keymaps are available, but it is recommended that you define your own. To define your own keymap copy the `keymap_base.c` file and name it `keymap_<name>.c`.
