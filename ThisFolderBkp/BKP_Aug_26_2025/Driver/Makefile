# Define the target driver name and object files
TARGET = ApoorvaPciDriver
OBJS = ApoorvaPciDriver.o

# Directory for kernel modules and current directory
MDIR = drivers/misc
PWD = $(shell pwd)
CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

# Compiler flags
EXTRA_CFLAGS = -DEXPORT_SYMTAB
ccflags-y += $(EXTRA_CFLAGS)

# The kernel build system recognizes the object file name with a .o extension
obj-m := $(TARGET).o

# Default target: build the module using the kernel build system
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Link the object file into a kernel object file (.ko)
$(TARGET).ko: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET).ko $(OBJS) -shared -fPIC

# Install the kernel module
install:
	# Copy the built kernel object file to the destination and update dependencies
	sudo cp -v $(TARGET).ko $(DEST)
	sudo /sbin/depmod -a

# Clean up the build directory
clean:
	# Clean up object files and kernel module
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c

# Include kernel build rules
-include $(KDIR)/Rules.make

