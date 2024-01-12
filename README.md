# RP1 launch second core

This hack hooks a frequently called function in the RP1 firmware, and
uses it to launch the second core at a known address.

Beware: this hardcodes the address of the function to hook, so if you
aren't running the same firmware version as me it might need modification.

My version is

    $ vcgencmd bootloader_version
    2024/01/05 15:57:40
    version 30cc5f37d65b279e820adb1d9840ad3c1cc98229 (release)

The current code blinks an LED connected to GPIO 17.

## Building and running

You'll need to install the arm-none-eabi toolchain to build RP1 binaries:

    sudo apt install binutils-arm-none-eabi

Then build everything:

    make

Load the hook code for core0 and the blinky code for core1:

    sudo ./mmio_load

Enable the hook:

    sudo ./mmio_enable_hook

If everything's working GPIO 17 should now be blinking, driven by the RP1 core1.
