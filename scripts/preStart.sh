#!/bin/sh

echo "Running fpp-plugin-notification PreStart Script"

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..
make
