module="ApoorvaPciDriver"
devnam="APOORVA1553"
device="APOORVA1553"
insmod ./$module.ko $* || exit 1
major=`awk " \\$2== \"$devnam\" {print \\$1} " /proc/devices`
mknod -m 0666 /dev/${device} c $major 0 
