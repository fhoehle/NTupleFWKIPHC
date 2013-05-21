#!/bin/bash
export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.27.04/x86_64-slc5-gcc43-opt/root
export PATH=${ROOTSYS}/bin:$PATH
export LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}
export ROOT_INCLUDE=${ROOTSYS}/include
