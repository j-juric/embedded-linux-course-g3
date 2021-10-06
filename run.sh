#!/bin/sh

MODE=
EXE_FOLDER=
DEV_MODE=
SHIFT=


# Gather flag information
TEMP=`getopt --long -o "s:tn" "$@"`
eval set -- "$TEMP"
while true ; do
    case "$1" in
#case "${flag}" in
        -n) MODE="Normal mode"; EXE_FOLDER="Release"; DEV_MODE="d"; shift;;
        -t) MODE="Test mode"; EXE_FOLDER="Debug"; DEV_MODE="t"; shift;;
        -s) SHIFT=$2; shift 2;;
        :) echo "Missing argument. Hint -n for normal mode, -t for test mode."; exit 1;;
        \?)  echo "Unknown argument. Hint -n for normal mode, -t for test mode."; exit 1;;
        *) break;;
    esac
done


echo "$MODE"

# Run kernel module
rmmod encrypter.ko
sleep 1
insmod ./dev_src/encrypter.ko mode=${DEV_MODE} shift=${SHIFT}
mknod /dev/encrypter c 60 0

# Ryb application
./user_bin/${EXE_FOLDER}/app | tee log.txt

echo "Done."