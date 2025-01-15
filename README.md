# NetKernel Project
Started this repository as a collection of my hobby projects and an attempt to dive deeper into linux kernel module development and the netfilter library.

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

## Character Driver Linux Kernel Module (module_char_driver)
Test module for demonstrating read and write for character driver (`/dev`). 

### Testing Module
```
# Enable kernel logs 
sudo dmesg -w &

# Load Module
sudo insmod module_char_driver.ko

# Read from /dev
cat /dev/char_driver

# Reset counters
echo "HelloWorld from Char driver" > /dev/char_driver
cat /dev/char_driver

# Unload module
sudo rmmod module_char_driver.ko
```


## Packet Counter Linux Kernel Module (packet_counter)
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


## L2 Switch Kernel Module (packet_counter)
Layer-2 software switch to forward Ethernet frames based on MAC addresses, utilizing Linux kernel networking APIs and Netfilter for packet processing and forwarding. Implemented MAC address learning along with and dynamic forwarding logic.

### Testing Module
```
# Enable kernel logs 
sudo dmesg -w &

# Load Module
sudo insmod l2_switch.ko

# Test with ping ( until 2 responses)
ping 127.0.0.1 -c 2

# Unload module
sudo rmmod l2_switch
```

### TODOs
- [ ] Test module using virtual networks
- [ ] Add ageing time on mac_table. Discard entires based on ageing time


