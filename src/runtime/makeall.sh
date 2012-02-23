#!/bin/bash
# called by src/

JOBS="4"

if [ $1 == "SHARED" ]; then
  SHARED=1 make -k -C runtime/tmp all -j $JOBS
else
  COMPILE=1 make -k -C runtime/tmp all -j $JOBS
  LINK=1 make -k -C runtime/tmp all -j $JOBS
fi
