# FAST CONVERTER
## Usage
Can run as executable or can be built as shared library

### For executable
make
./build/fastconverter.exe -f models/cow.obj
OR
./build/fastconverter.exe

### For shared library
make shared
link with build/libfastconverter.so

## Key Takeaways
Can work as standalone or as a module
use vector math library opengl math library glm
can convert OBJ data to STL file