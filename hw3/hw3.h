#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#define SIZE 1100000000
#define FLUSHPERIOD 1