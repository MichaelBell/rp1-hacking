# PIO documentation

This documents what we know so far about the PIO implementation on RP1 - unfortunately the section is currently missing from the official datasheet.

Overall, the PIO unit is believed to be very similar to the RP2040.  There is a single instance with 32 instruction words (same as RP2040), 4 SMs (same as RP2040) and 8 deep FIFOs (double the RP2040).

The PIO registers are accessible at address `0xf000_0000` from the RP1.  Additionally the FIFOs (only) are accessible at `0x40178000` and hence from Linux (at `0x1f_00178000`).

This means that configuring the PIO requires running code on the RP1.  Until Raspberry Pi provide a supported way to do that, this unfortunately means any use of PIO will rely on patching the running firmware on the RP1, which is far from ideal!

## RP1 register documentation

The register layout at `0xf000_0000` is general very similar to the RP2040.  Differences are documented below - for the known registers refer to section 3.7 of the RP2040 datasheet.

| Offset  | Register | Notes |
| ------  | -------- | ----- |
| 0x00    | CTRL     | No known differences from RP2040 |
| 0x04    | FSTAT    | No known differences from RP2040 |
| 0x08    | FDEBUG   | No known differences from RP2040 |
| 0x0c    | FLEVEL   | When joined, the FIFOs maximum level is 16.  The levels here read as 15 when the level is 15 or 16. |
| 0x10    | FLEVEL2  | Same layout as FLEVEL, but showing the top bit of the level, so 1 when the level is 16, zero otherwise. |
| 0x14 - 0x20 | TX FIFOs | Same as RP2040, also accessible at `0x40178000` |
| 0x24 - 0x30 | RX FIFOs | Same as RP2040, also accessible at `0x40178010` |
| 0x34    | IRQ      | Likely the same as RP2040 - not tested |
| 0x38    | IRQ_FORCE | Likely the same as RP2040 - not tested |
| 0x3c    | INPUT_SYNC_BYPASS | Likely the same as RP2040 - not tested |
| 0x40    | DBG_PADOUT | No known differences from RP2040 |
| 0x44    | DBG_PADOE | No known differences from RP2040 |
| 0x48    | DBG_CFGINFO | No known differences from RP2040 |
| 0x4c - 0xc8 | INSTR_MEM | No known differences from RP2040 |
| 0xcc, 0xec, 0x10c, 0x12c | SMx_CLKDIV | No known differences from RP2040 |
| 0xd0, etc | SMx_EXECCTRL | Bit 5 (reserved in RP2040) is settable, but has unknown function |
| 0xd4, etc | SMx_SHIFTCTRL | No known differences from RP2040 |
| 0xd8, etc | SMx_ADDR | No known differences from RP2040 |
| 0xdc, etc | SMx_INSTR | No known differences from RP2040 |
| 0xe0, etc | SMx_PINCTRL | No known differences from RP2040 |
| 0xe4, etc | SMx_DMATX | 31: Enable DREQ, 30: Current DREQ status, 11-7: Unknown, 4-0: DREQ threshold (DREQ asserted when TX FIFO below this level) |
| 0xe8, etc | SMx_DMARX? | (Speculative) 31: Enable DREQ, 30: Current DREQ status, 11-7: Unknown, 4-0: Unknown |
| 0x14c     | INTR | No known differences from RP2040 |
| 0x150 - 0x168 | Interrupt control | Likely the same as RP2040 - not tested |

`0x16c` reads as `pio2` in ASCII.

The bottom bit of `0x170` and `0x178` is settable.  And there are similar settable bits at offsets `0x28` and `0x30` in the `0x40178000` space, but they aren't the same register (writes to one are not reflected in the other).  Their function is unknown.
