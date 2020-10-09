#!/bin/sh
SRC=$(find src/ -type f -name '*.cpp')
INCLUDE=$(find include/ -type f -name '*')
RESOURCES=$(find resources/ -type f -name '*')
TOOLS_SRC=$(find tools/ -type f -name '*.cpp')
MAC=$(find mac/ -type f -name '*')
MAKE_FILES="build.sh compress.sh embedResources.sh Makefile Makefile.linux.mk Makefile.windows.mk"
if [ $VSCODE_SETTINGS = "true" ]; then export VSCS="$(find .vscode/ -type f -name '*')"; fi
rm -f compress.tar 
tar -zcvf compress.tar $SRC $INCLUDE $RESOURCES $TOOLS_SRC $MAKE_FILES $MAC $VSCS
