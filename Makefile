all: mmio_view mmio_dump

clean:
	rm -f mmio_view

mmio%: mmio%.c
	gcc $< -o $@
