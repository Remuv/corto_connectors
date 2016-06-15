#!/bin/bash

PACKAGE_NAME='recorto'
CORTO_VERSION="0.2"

VERSION=0.0
BUILD=XX
RELEASE=MR
#HOME="$(getent passwd $USER | awk -F ':' '{print $6}')"
BUILD_DIR="/usr/local/"
INC_DIR="${BUILD_DIR}/include/corto/${CORTO_VERSION}/${PACKAGE_NAME}"
LIB_DIR="${BUILD_DIR}/lib/corto/${CORTO_VERSION}/${PACKAGE_NAME}"

####################
## Get Version Num
####################
filename="version.txt"
while read -r line
do
  VERSION=$line
done < "$filename"

####################
## Get Build Version
####################
if [ "$1" != "" ]; then
  BUILD=$1
fi
####################
## Get build release
####################
if [ "$2" != "" ]; then
  RELEASE=$2
fi

fpm \
    -s dir \
    -t deb \
    --name ${PACKAGE_NAME}-${RELEASE}-${BUILD} \
    --version ${VERSION} \
    --force \
    "${INC_DIR}" \
    "${LIB_DIR}"
