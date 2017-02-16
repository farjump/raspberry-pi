toolchain := arm-none-eabi
elfs      := hello.elf

# Hello World
sources/hello.elf := $(addprefix src/hello-world/, HelloWorld.c)

# Common Flags
cflags  := -specs=sdk/Alpha.specs -mfloat-abi=hard -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -g3 -ggdb
ldflags := -Wl,-Tsdk/link.ld -Lsdk

.PHONY: all
all: $(elfs)

.PHONY: clean
clean:
	rm -f $(elfs) $(addsuffix .map, $(basename $(elfs)))

.SECONDEXPANSION:
$(elfs): $$(sources/$$@)
	$(toolchain)-gcc $(cflags) $(ldflags) -Wl,-Map,$(basename $@).map -o $@ $(cflags/$@) $^ $(libs/$@)

.PHONY: shell
shell:
	docker build --build-arg uid=$$(id -u) .
	docker run -it -v $$PWD:$$PWD -w $$PWD --privileged $$(docker build -q --build-arg uid=$$(id -u) .)
