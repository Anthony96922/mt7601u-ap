# mt7601u-ap
AP driver for MT7601U chipset based adapters

Many usb Wi-Fi adapters that claim to have an RT5370 chipset actually have an MT7601 chipset. There are no drivers in Linux that support this device. This driver can be used to set up a hotspot with these devices. This has compiled on the following kernels:

- 3.12.35+ (Raspberry Pi)

### How to use
Get the sources for your kernel. You can do this by running:
````sh
$ sudo apt-get install linux-headers-generic
````
Then download the source and compile:
````sh
$ git clone https://github.com/anthony96922/mt7601u-ap
$ cd mt7601u-ap
$ make
$ sudo make install
$ sudo modprobe mt7601Uap
````

If the module has loaded then running `ifconfig ra0 up` will bring up a Wi-Fi access point that you can connect your devices to.
You can change the network name and the security by editing the file `/etc/wifi/RT2870AP/RT2870AP.txt`.

#### Cross compiling
You can cross compile this driver by setting `LINUX_SRC` to the kernel sources, `CROSS_COMPILE` to your compiler and `ARCH` to the arch you are compiling for. It is arm for the Raspberry Pi.

#### You are done!!
