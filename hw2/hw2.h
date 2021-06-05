#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 1000000
#define DIVIDE 1000
#define MAXTHREAD 24
const int thread_count = 2;
struct pos
{
    int min;
    int max;
    pos(){};
    pos(int min, int max) : min(min), max(max) {}
    bool operator==(const pos &posx)
    {
        return (min == posx.min && max == posx.max);
    }
};

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}