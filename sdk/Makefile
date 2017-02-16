GNU = arm-none-eabi
CFLAGS  = -mfloat-abi=hard -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -Og -g

all : libCPU.a

libCPU.a : CPU_start.o CPU_init.o CPU_init_util.o
	$(GNU)-ar -r libCPU.a CPU_init.o CPU_init_util.o

%.o:	%.c
	$(GNU)-gcc $(CFLAGS) $^ -c

%.o:	%.S
	$(GNU)-gcc $(CFLAGS) $^ -c


clean :
	rm *.o libCPU.a


