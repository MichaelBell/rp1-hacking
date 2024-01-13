all: oneshot.bin mmio_start

clean:
	rm *.o *.elf *.bin

%.o: %.s
	arm-none-eabi-as $< -o $@

%.o: %.c
	arm-none-eabi-gcc -nostdlib -nostartfiles -ffreestanding -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -Wall -c $< -o $@

oneshot.elf: start.o main.o uart.o
	arm-none-eabi-ld $^ -T memmap --gc-sections -o $@

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

mmio%: mmio%.c
	gcc $< -o $@
