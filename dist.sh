if [ ! -d "release" ]; then
  mkdir release
fi

cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release --target install -- -j 4