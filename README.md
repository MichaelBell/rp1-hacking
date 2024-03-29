# Running code on the RP1

This hack hooks a frequently called function in the RP1 firmware, and
uses it to run code without needing to reset the RP1.

Beware: this hardcodes the address of the function to hook, so if you
aren't running the same firmware version as me it might need modification.

My version is

    $ vcgencmd bootloader_version
    2024/01/05 15:57:40
    version 30cc5f37d65b279e820adb1d9840ad3c1cc98229 (release)

## Second core launch

This uses the hook to launch the second core (core 1, the first core is core 0) at `0x20008000`.

### Building and running

You'll need to install the arm-none-eabi toolchain to build RP1 binaries:

    sudo apt install binutils-arm-none-eabi

Then build everything:

    cd launch_core1
    make

Load the blinky example and run it on core1:

    sudo ./core1_test -f blink_core1.bin

If everything's working GPIO 17 should now be blinking, driven by the RP1 core1.

## One shot

This runs a program once on core 0.  The program should exit quickly, so that core0 can get back to its normal tasks.

Everything is returned to normal after the program runs, so this can be done multiple times.

The current program toggles the state of GPIO 17.

### Building and running

You'll need to install the arm-none-eabi toolchain to build RP1 binaries:

    sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi

Then build everything:

    make

Load the hook code for core0 and the blinky code for core1:

    sudo ./mmio_start

## Other information

Details about the [PIO](PIO.md) registers (found at `0xf000_0000` from the RP1).

The 3 RIO register banks can be found at `0xf000_4000`, `0xf000_8000` and `0xf000_c000`, giving private uncontested access from the RP1.  These corresponding to the 3 GPIO banks on the RP1.  (They are also accessible at `0x400e0000`, `0x400e4000` and `0x400e8000`, as documented in the datasheet.)

## Credits

This work builds on the reverse engineering work done by [G33katWork](https://github.com/G33KatWork/RP1-Reverse-Engineering/)

Thanks to @clever on the Raspberry Pi discord for help and inspiration while getting this going.
