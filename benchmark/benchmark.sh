#!/bin/bash

#linux-specific script for running benchmarks

set -e
set -o pipefail

drop_caches()
{
  echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null
}

print_separator()
{
    echo "-------------------------------------"
}

realtime()
{
    echo "$*" | sudo chrt --rr 99 su $USER -c bash
}

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 N"
    exit 1
fi

N=$1
sudo echo "N = $N" # to warmup sudo

cmake -DBENCHMARK_N=$N .
#cmake --build . --target frozen
#cmake --build . --target hayai
rm -f benchmark/strswitch_benchmark
rm -f benchmark/frozen_benchmark
rm -f benchmark/CMakeFiles/strswitch_benchmark.dir/frozen_benchmark.cpp.o
rm -f benchmark/CMakeFiles/frozen_benchmark.dir/frozen_benchmark.cpp.o

print_separator

drop_caches
realtime /usr/bin/time "--format='time %E mem %M\n'" cmake --build . --target frozen_benchmark

print_separator

drop_caches
realtime /usr/bin/time "--format='time %E mem %M\n'" cmake --build . --target strswitch_benchmark

print_separator

drop_caches
realtime benchmark/frozen_benchmark

print_separator

drop_caches
realtime benchmark/strswitch_benchmark
