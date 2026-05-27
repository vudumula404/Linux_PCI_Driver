#!/bin/bash

module="ApoorvaPciDriver"
devname="APOORVA1553"
device="APOORVA1553"
libfile="libApoorva31Rts_4NodeDll.a"

# Copy library
cp ./$libfile /usr/lib || { echo "Failed to copy $libfile"; exit 1; }

# Insert kernel module
insmod ./$module.ko $* || { echo "Failed to insert $module.ko"; exit 1; }

# Get major number
major=$(awk "\$2==\"$devname\" {print \$1}" /proc/devices)

if [ -z "$major" ]; then
    echo "Device $devname not found in /proc/devices"
    exit 1
fi

# Create device node
rm -f /dev/${device}
mknod -m 0666 /dev/${device} c $major 0

echo "Device node /dev/${device} created with major number $major"

