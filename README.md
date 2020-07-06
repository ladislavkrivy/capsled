# capsled
### Capslock led mapping for ThinkPad T430, T440, X230, X240

#### Build

To build this software on Ubuntu and Debian systems just run `make`.
Other systems might not be supported. If you manage to compile capsled on other distros please update capsled.c and the Makefile and submit a pull request.

#### Execute

This software needs to be run as root/with root privileges for reading the capslock keystrokes and controlling the power led.

#### Install

Running `make install` (as root) installs the software to usr/bin/ and creates and enables it's service in /etc/systemd/system/

#### Choice of led

This program uses /proc/acpi/ibm/led to command the desired led and it uses led 0, which is the power button LED.
Other led's could be possible, like sound mute or mic mute led. Most of other leds become only available after enabling CONFIG_THINKPAD_ACPI_UNSAFE_LEDS when compiling kernel (module).
Instead of led 0 as is used now one could by making small adjustments to this code, by choosing another led number.