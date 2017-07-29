#!/bin/bash
#set -x

# This script provides the quickest means to convert code
# from the original wolfssl source tree to code that makes
# up the ported Particle wolfssl library.  This will
# put things in the supported v2 structure.
##

# Save the operating ROOT directory
##
ROOT=`pwd`
PARTICLE_PROJECT_DIR=/Users/cermak/Particle/libraries

# Define a main source directory (SRC)
# Define the target Particle project directory (PPD)
# New code not found in the source directory (ORG)
SRC=${ROOT}/wolfssl-3.11.0
ORG=${ROOT}/wolfssl-custom
PPD=${PARTICLE_PROJECT_DIR}/wolfssl/src

# Special patch files
##
PATCHDIR=${ROOT}/wolfssl-patches

# Original particle project tree
##
ORIG=""

# wolfssl provides a way to restructure the source code
# into the Arduino IDE format.  This will give us a 
# head start
##
cd ${SRC}
cd IDE/ARDUINO
./wolfssl-arduino.sh

# Copy and rename main source
##
cd ${SRC}
for i in $(ls *.c); do
  cp ${i} ${PPD}/${i}pp
done

# Copy header structures into src/
##
cp -a wolfssl ${PPD}/

