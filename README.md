# mt7601u-ap

Feel free to create pull requests and I will gladly accept them!

AP driver for MT7601U chipset based adapters

Many usb Wi-Fi adapters that claim to have an RT5370 chipset actually have an MT7601 chipset. There are no drivers in Linux that support this device. This driver can be used to set up a hotspot with these devices. This has compiled and works on the following kernels:

- 3.10.103 (Odroid C1/C1+)
- 3.12.35 (Raspberry Pi)
- 3.18.16 (Raspberry Pi)
- 4.4.12 (Raspberry Pi)

Please note that this kernel will only work properly on 32 bits. It will crash on 64 bits.

### How to use
Get the sources for your kernel. You can do this by running:
````sh
$ sudo apt-get install linux-headers-generic
````
Then download the source and compile:
````sh
$ git clone https://github.com/tanaka1892/mt7601u-ap
$ cd mt7601u-ap
$ make
$ sudo make install
$ sudo modprobe mt7601Uap
````

If the module has loaded then running `ifconfig ra0 up` will bring up a Wi-Fi access point that you can connect your devices to.
You can change the network name and the security by editing the file `/etc/wifi/RT2870AP/RT2870AP.txt`.

#### Cross compiling
You can cross compile this driver by setting `LINUX_SRC` to the kernel sources, `CROSS_COMPILE` to your compiler and `ARCH` to the arch you are compiling for. It is arm for the Raspberry Pi.

##### Cross compiling on a virtual machine for Raspberry Pi (Xubuntu 14.04 32 bits)
Download the kernel sources and cross compiler:
````sh
$ git clone https://github.com/raspberrypi/linux -b rpi-4.4.y
$ git clone https://github.com/raspberrypi/tools
$ sudo mv tools /opt/
$ nano .bashrc
````
Add this line:
````
export PATH=/opt/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin:$PATH
````
Then update the PATH:
````sh
$ . .bashrc
````
Prepare the kernel sources:
````sh
$ cd linux
$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcmrpi_defconfig
$ make modules_prepare
````
Compile the driver:
````
$ cd ../mt7601u-ap
$ make LINUX_SRC=../linux ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
````
The module name is mt7601Uap.ko and will be in `os/linux/` when compiling is done. Copy it over to `/lib/modules/<kernel version>/kernel/drivers/net/wireless` and run `depmod $(uname -r)` to make it load on startup.
#### You are done!!
