#!/bin/bash

SO_FILES=$(find bazel-bin/stratum/ -name *.so -type f)

touch _coverage.lcov

for f in $SO_FILES
do
  llvm-cov export --format lcov --instr-profile _coverage.dat $f >> _coverage.lcov
  echo '<<<<<< EOF' >> _coverage.lcov
done
