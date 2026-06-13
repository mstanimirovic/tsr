
cflags += -Wall -Wextra -pedantic -I./ -Iexamples $(shell pkg-config --cflags sdl3)
ldflags += $(shell pkg-config --libs sdl3)
example_srcs := $(wildcard examples/*.c)
example_bins := $(patsubst %.c,%.bin,$(example_srcs))

default: $(example_bins)

examples/%.bin: examples/%.c examples/tsr_sdl3.h tsr.h
	cc $(cflags) $< -o $@ $(ldflags)

bin:
	@mkdir -p $@

.PHONY: clean
clean:
	@rm -rvf examples/*.bin
