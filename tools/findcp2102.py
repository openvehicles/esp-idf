#!/usr/bin/env python
#
# This program is a workaround for the inconsistent device naming
# pattern implemented in the device driver for Silicon Labs CP2102 USB
# to UART Bridge Controller.  When there are multiple devices
# installed on one computer, the second and subsequent devices are
# given a numeric suffix that increments every time one of the devices
# is plugged in.  Thus, unplugging and reinserting a cable causes the
# device name to change.
#
# The workaround is to use the device USB location identifier as a
# consistent handle for the device, assuming that the cable is plugged
# into the same jack each time.
#
# This program has two uses:
#
# 1) When invoked with no arguments it searches for all CP2102 devices
# in the computer's USB hierarchy and prints a list of the bus
# location identifier and device name for each.  The device names will
# be numbered in the order they were plugged in, so this list allows
# associating a location identifier with each physical device.
#
# 2) When invoked with one argument that is the bus location of a
# CP2102 device it prints the associated device name.  This allows the
# location identifier to be used as a constant identifier in a system
# configuration file and then this program can be called in a script or
# makefile to translate to the dynamic device name.  If the argument
# is not found as a location identifier, the argument will be output
# unchanged; this allows the script or makefile to work with either a
# location identifier or a device name as the configured handle.
#
# This program is derived from code provided without copyright by user
# spachner21 in the Silicon Labs Interface Forum.  It is provided here
# as public domain.

import sys
import serial.tools.list_ports

ports = serial.tools.list_ports.grep("USB VID:PID=10c4:ea60")

#Make new list from generated 'generator' objects returned by grep above
pl = []
for p in ports:
    pl.append(p)

#Sort list by object attribute 'location'
pl.sort(key=lambda x: x.location, reverse=False)

if len(sys.argv) > 1:
    for p in pl:
        if str(p.location) == sys.argv[1]:
            print p.device
            break
    else:
        print sys.argv[1]
else:
    for p in pl:
        print p.location + " " + p.device
