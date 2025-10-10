# Python
import ftd3xx #need to switch the driver to the non wu version

print("hello world")
# List all connected FT3xx devices
devices = ftd3xx.listDevices()
print("Connected FT3xx Devices:", devices)