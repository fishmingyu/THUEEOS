flag=-std=c++11
loadlib="-lrt"
path="bin"
g++ $flag -o $path/updown1 updown1.cpp $loadlib
g++ $flag -o $path/updown2 updown2.cpp $loadlib
g++ $flag -o $path/genData genData.cpp $loadlib
