all: launch_core1.bin core1.bin blink_core1.bin mmio_enable_hook mmio_load mmio_view

clean:
	rm *.o *.elf *.bin mmio_enable_hook mmio_disable_hook mmio_load mmio_view

%.o: %.s
	arm-none-eabi-as $< -o $@

launch_core1.elf: launch_core1.o
	arm-none-eabi-ld launch_core1.o -T memmap -o $@

core1.elf: count_core1.o
	arm-none-eabi-ld count_core1.o -T memmap_core1 -o $@

blink_core1.elf: blink_core1.o
	arm-none-eabi-ld blink_core1.o -T memmap_core1 -o $@

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

mmio%: mmio%.c
	gcc $< -o $@
