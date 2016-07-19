#!/bin/bash

PACKAGE_NAME='recorto'
CORTO_VERSION="0.2"

VERSION=0.0
BUILD=XX
RELEASE=MR
BUILD_DIR="/usr/local"
INC_DIR="${BUILD_DIR}/include/corto/${CORTO_VERSION}/${PACKAGE_NAME}"
LIB_DIR="${BUILD_DIR}/lib/corto/${CORTO_VERSION}/${PACKAGE_NAME}"
INPUTS_FILE="`pwd`/lib_input.txt"
THIRD_PARTY_LIB="/usr/local/lib/*.so*"

echo "INPUTS_FILE=${INPUTS_FILE}"

display_usage() {
	echo "package.sh <BUILD_VERSION> <BUILD_RELEASE> <DEP_DIR>"
}

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

####################
## REMOVE Dep PKGs
####################
removePackages()
{
  BUFFER=`sudo dpkg --info ${1} | grep Package: `
  PACKAGE=$(echo ${BUFFER} | awk '{ print $2 }')
  sudo dpkg --purge ${PACKAGE}
}

DIR=${3}
if [ "${DIR}" = "." ]
then
  DIR=`pwd`
fi

export -f removePackages
find ${DIR}/* -prune  -iname "*.deb" -exec bash -c 'removePackages  "$0"' {} \;

####################
## BUILD INPUTS_FILE
####################
if [ -f ${INPUTS_FILE} ]
then
  rm ${INPUTS_FILE}
fi
find /usr/local/lib/* -prune  -iname "*.so*" -exec echo {} >> ${INPUTS_FILE} \;
cat ${INPUTS_FILE}

####################
## Package
####################

fpm \
    -s dir \
    -t deb \
    --name ${PACKAGE_NAME}-${RELEASE}-${BUILD} \
    --version ${VERSION} \
    --force \
    --inputs ${INPUTS_FILE} \
    "${INC_DIR}" \
    "${LIB_DIR}"

rm ${INPUTS_FILE}
