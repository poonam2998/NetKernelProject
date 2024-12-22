# Module name and objects
obj-m := l2_switch.o
l2_switch-objs := src/l2_switch_main.o src/packet_handler.o src/mac_table.o

# Add subdirectory for headers and enable debug flags
EXTRA_CFLAGS += -I$(PWD)/include -g -DDEBUG

# Build and clean targets
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

