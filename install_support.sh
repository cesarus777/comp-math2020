#!/bin/bash

if bash -- ./support/install.sh $* && [[ ${LD_LIBRARY_PATH} != *"/install/support/"* ]]; then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/install/support/
fi

