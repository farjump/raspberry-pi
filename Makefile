toolchain := arm-none-eabi
elfs      := hello.elf raytracer.elf

# Hello World
sources/hello.elf := $(addprefix src/hello-world/, HelloWorld.c)

# Raytracer
sources/raytracer.elf := $(addprefix src/raytracer/, main.c Raytracing.c VC.c VC_aligned_buffer.S)
libs/raytracer.elf    := -lm
cflags/raytracer.elf  := -Og

# Common Flags
cflags  := -specs=sdk/Alpha.specs -mfloat-abi=hard -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -g3 -ggdb
ldflags := -Wl,-Tsdk/link.ld -Lsdk

.PHONY: all
all: $(elfs)

.PHONY: clean
clean:
	rm -f $(elfs) $(addsuffix .map, $(basename $(elfs))) kernel.img kernel.dbg

kernel.img: raytracer.elf
	$(toolchain)-objcopy -O binary $< $@
	$(toolchain)-objcopy --only-keep-debug $< $(basename $@).dbg

.SECONDEXPANSION:
$(elfs): $$(sources/$$@)
	$(toolchain)-gcc $(cflags) $(ldflags) -Wl,-Map,$(basename $@).map -o $@ $(cflags/$@) $^ $(libs/$@)

.PHONY: shell
shell:
	docker build --build-arg uid=$$(id -u) .
	docker run -it -v $$PWD:$$PWD -w $$PWD --privileged $$(docker build -q --build-arg uid=$$(id -u) .)
