flag=-std=c++11
loadlib="-lpthread -lrt"
path="bin"
g++ $flag -o $path/shm shm.cpp $loadlib
g++ $flag -o $path/genData genData.cpp $loadlib
g++ $flag -o $path/verify verifyData.cpp $loadlib