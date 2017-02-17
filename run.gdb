source sdk/alpha.gdb

# Connect to the RPi
set serial baud 115200
target remote /dev/ttyUSB0

# Load the executable in the target
# By default, the loaded file is the one GDB debugs, given as argument
# to gdb or using the command file.
load

# Run untile reaching main
tbreak main
continue
