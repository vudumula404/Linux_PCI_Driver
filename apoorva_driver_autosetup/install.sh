#!/bin/bash

echo "🔧 Installing Apoorva PCI Driver and libraries..."

# Load the driver

echo "📦 Inserting kernel module..."
sudo insmod ApoorvaPciDriver.ko

# Create the device node
echo "🧱 Creating /dev/APOORVA1553..."
sudo mknod /dev/APOORVA1553 c 236 0
sudo chmod 666 /dev/APOORVA1553

# Copy libraries
echo "📚 Copying static libraries..."
sudo cp libApoorva31Rts_4NodeDll.a /usr/lib/
sudo cp libApoorva31Rts_4NodeDll.o /usr/lib/

# Update shared library cache (precautionary)
sudo ldconfig

echo "✅ Installation complete."


