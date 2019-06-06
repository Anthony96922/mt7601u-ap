# mt7601u-ap

AP driver for MT7601U chipset based adapters

Many USB Wi-Fi dongles advertised to have an RT5370 chipset actually have an MT7601 chipset. There is not driver in Linux that support these devices in AP mode. This driver can be used to set up a hotspot with these devices. This should compile on most kernels without issue if the API's haven't been changed. If you're running Ubuntu or any of its derivatives, the kernels are most likely modified and may cause compilation errors.

Please create a pull request if you've worked on cleaning up some of the code as it is currently a mess to read.

### How to build
````sh
$ git clone https://github.com/tanaka1892/mt7601u-ap
$ cd mt7601u-ap
$ make
$ sudo make install
$ sudo modprobe mt7601Uap
````

If the module has loaded then running `ip link set ra0 up` will bring up a Wi-Fi access point that you can connect your devices to.
You can change the network name and the security by editing the file `/etc/wifi/RT2870AP/RT2870AP.txt`.

#### Cross compiling
This can be built for a different platform by setting `LINUX_SRC` to the kernel sources, `CROSS_COMPILE` to your compiler and `ARCH` to the architecture you are compiling for.
