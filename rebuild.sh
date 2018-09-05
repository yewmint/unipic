if [ ! -d "debug" ]; then
  mkdir debug
fi

cd debug
cmake -DCMAKE_BUILD_TYPE=Debug -DGENERATE_UNIT_TEST=On ..
cmake --build . --config Debug --target install -- -j 4
