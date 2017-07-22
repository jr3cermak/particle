# local toolchain

# building

`make all PLATFORM=photon APPDIR=../../${PROJECT}`

# serial flashing

## MacOS

```
stty -f /dev/cu.usbmodem* speed 28800 > /dev/null
particle flash --serial ${PROJECT}/target/${PROJECT}.bin
particle serial monitor --follow
```
