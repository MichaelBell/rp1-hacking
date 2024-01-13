all: mmio_view

clean:
	rm -f mmio_view

mmio%: mmio%.c
	gcc $< -o $@
