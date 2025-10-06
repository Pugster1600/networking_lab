# Python
import ftd3xx

print("hello world")
# List all connected FT3xx devices
devices = ftd3xx.listDevices()
print("Connected FT3xx Devices:", devices)