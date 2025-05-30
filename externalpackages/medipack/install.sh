#!/bin/bash
set -eu

PREFIX="${ISSM_DIR}/externalpackages/medipack/install" # Set to location where external package should be installed

# Cleanup
rm -rf ${PREFIX}
mkdir -p ${PREFIX}

#Download development version
git clone https://github.com/SciCompKL/MeDiPack.git install
