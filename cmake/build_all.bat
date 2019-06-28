cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug  .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release  .
make -j4
arm-none-eabi-objcopy -O ihex debug/ros_k60.elf k60_flash.hex
IF "%1" == "" ( pause )
