#!/bin/sh

ROOT_DIR=`pwd`
INC_TARGET=${ROOT_DIR}/include
LIB_TARGET=${ROOT_DIR}/install/Linux-x86_64/lib

BSON_DIR=${ROOT_DIR}/libbson
BSON_LIB_DIR=${BSON_DIR}/.libs
BSON_INC_DIR=${BSON_DIR}/src/bson

MONGO_C_DRIVER_DIR=${ROOT_DIR}/mongo-c-driver
MONGO_C_DRIVER_LIB_DIR=${MONGO_C_DRIVER_DIR}/.libs
MONGO_C_DRIVER_INC_DIR=${MONGO_C_DRIVER_DIR}/src/mongoc

MONGO_DIR=${ROOT_DIR}/mongo-cxx-driver

### Ensure Submodules are current
git submodule update --init --recursive

### Build BSON Dir
cd ${BSON_DIR}
echo Build libbson
./autogen.sh
make
sudo make install
cp ${BSON_LIB_DIR}/*.so* ${LIB_TARGET}

### Build Mongo Dir
echo Build mongo c driver
cd ${MONGO_C_DRIVER_DIR}
./autogen.sh
make
sudo make install

cp ${MONGO_C_DRIVER_LIB_DIR}/*.so* ${LIB_TARGET}

### Build MongoCXX
echo Build MongoCXX
cd ${MONGO_DIR}
cmake -DCMAKE_BUILD_TYPE=Release -DDESTDIR=${MONGO_DIR}/install_dir
make && sudo make install

if [ -d ${INC_TARGET}/bsoncxx ]
then
rm -r ${INC_TARGET}/bsoncxx/*
fi

if [ -d ${INC_TARGET}/mongocxx ]
then
rm -r ${INC_TARGET}/mongocxx/*
fi

cp -r ${MONGO_DIR}/install/include/bsoncxx/v_noabi/* ${INC_TARGET}/
cp -r ${MONGO_DIR}/install/include/mongocxx/v_noabi/* ${INC_TARGET}/
cp ${MONGO_DIR}/install/lib/*.so* ${LIB_TARGET}/
