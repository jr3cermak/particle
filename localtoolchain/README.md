# Local Toolchain

# Building

```
cd ${SRC}/firmware/modules
make all PLATFORM=photon APPDIR=../../${PROJECT}
````

# Flashing

## MacOS (Serial)

```
stty -f /dev/cu.usbmodem* speed 28800 > /dev/null
particle flash --serial ${PROJECT}/target/${PROJECT}.bin
particle serial monitor --follow
```
