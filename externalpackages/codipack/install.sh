#!/bin/bash
set -eu

VER=1.8
PREFIX="${ISSM_DIR}/externalpackages/codipack/install" # Set to location where external package should be installed

# Cleanup
rm -rf ${PREFIX} CoDiPack-$VER.tar.gz
mkdir -p ${PREFIX}

#Download development version
git clone https://github.com/SciCompKL/CoDiPack.git install

## Download source
#$ISSM_DIR/scripts/DownloadExternalPackage.sh "https://issm.ess.uci.edu/files/externalpackages/CoDiPack-${VER}.tar.gz" "CoDiPack-${VER}.tar.gz"
#
## Untar
#tar -zxvf CoDiPack-$VER.tar.gz
#
## Move source into install directory
#mv CoDiPack-$VER install
#rm -rf CoDiPack-$VER/
