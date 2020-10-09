#!/bin/sh
cd builds
rm .objs/target/linux/Core/WindFramework.o
rm .objs/target/linux/Core/WindFramework.d
sh embedResources.sh
make linux DMODE=1 -j8