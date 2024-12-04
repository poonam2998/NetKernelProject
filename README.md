# PacketLogger

## Hello World Linux Kernel Module (module_hello)
Test module for demonstrating basic module functionality and reading from proc file system. It prints messages in kernel log on loading and unloading. 

### Testing Module
```
# Enable kernel logs 
sudo dmesg -w &

# Load Module
sudo insmod module_hello.ko

# Read from /proc
cat /proc/helloworlddriver

# Unload module
sudo rmmod module_hello
```

## Packet Counter Linux Kernel Module (module_hello)
Module for counting and logging TCP, UDP and ICMP network packets. It demonstates how to hook into the linux networking stack using the `Netfilters` library and provides packet stats using `/proc` interface. 

### Testing Module
```
# Enable kernel logs 
sudo dmesg -w &

# Load Module
sudo insmod packet_counter.ko

# Read from /proc
cat /proc/packet_counter

# Reset counters
echo reset > /proc/packet_counter
cat /proc/packet_counter

# Unload module
sudo rmmod packet_counter
```
