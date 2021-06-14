#include "hw3.h"

int main()
{
    struct timeval start;
    struct timeval end;
    int fd = open("bin/test.bin", O_RDWR);
    if (fd < 0)
    {
        printf("open failed!\n");
        return -1;
    }
    char *data = (char *)mmap(NULL, SIZE * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (!data)
    {
        printf("mmap failed\n");
        close(fd);
    }
    gettimeofday(&start, NULL);
    for (int i = 0; i <= SIZE - i - 1; i++)
    {
        char c = data[i];
        data[i] = data[SIZE - i - 1];
        data[SIZE - i] = c;
    }
    gettimeofday(&end, NULL);
    int time_use = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("time_use %d us\n", time_use);
    msync(data, SIZE * sizeof(int), MAP_SHARED);
    munmap(data, SIZE);
    close(fd);
    return 0;
}