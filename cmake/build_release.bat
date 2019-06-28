cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release  .
make -j4
IF "%1" == "" ( pause ) 
