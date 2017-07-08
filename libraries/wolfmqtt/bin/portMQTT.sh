#!/bin/bash
#set -x

# This script provides the quickest means to convert code
# from the original wolfmqtt source tree to code that makes
# up the ported Particle wolfmqtt library.  This will
# put things in the supported v2 structure.
##

# Save the operating ROOT directory
##
ROOT=`pwd`
PARTICLE_PROJECT_DIR=/Users/jensa/Dropbox/Kunder/OrderX/OrderX_V3/

# Define a main source directory (SRC)
# Define a main header directory (DOTH)
# Define the target Particle project directory (PPD) (Note that these folders have to exist in the file system)
SRC=${ROOT}/wolfMQTT_org/src
DOTH=${ROOT}/wolfMQTT_org/wolfmqtt
PPD=${PARTICLE_PROJECT_DIR}/wolfmqtt/src/wolfmqtt

# Copy and rename main source
##
cd ${SRC}
for i in $(ls *.c); do
  cp ${i} ${PPD}/${i}pp
done

# Copy header structures into src/
##

cd ${DOTH}
for i in $(ls *.h); do
  cp ${i} ${PPD}/${i}
done

# make the Arduino header file
echo "/* Generated wolfMQTT header file for Arduino */" >> ${PPD}/wolfMQTT.h
echo "#include <wolfmqtt/mqtt_client.h>" >> ${PPD}/wolfMQTT.h