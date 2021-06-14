#include "hw3.h"

int main()
{
    struct timeval start_time;
    struct timeval end_time;
    char *start = (char *)malloc(2 * sizeof(char));
    char *end = (char *)malloc(2 * sizeof(char));
    FILE *fp = fopen("bin/test.bin", "r+");

    if (fp == 0)
    {
        perror("error opening source file.\n");
        exit(0);
    }
    char *data_in = (char *)malloc(SIZE * sizeof(char));
    char *data_out = (char *)malloc(SIZE * sizeof(char));
    fread(data_in, sizeof(char), SIZE, fp);
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < SIZE / 2; i++)
    {
        fseek(fp, i, SEEK_SET);
        fread(start, sizeof(char), 1, fp);
        fseek(fp, SIZE - i - 1, SEEK_SET);
        fread(end, sizeof(char), 1, fp);
        fseek(fp, SIZE - i - 1, SEEK_SET);
        fwrite(start, sizeof(char), 1, fp);
        fseek(fp, i, SEEK_SET);
        fwrite(end, sizeof(char), 1, fp);
        if (i % FLUSHPERIOD)
            fflush(fp);
    }
    gettimeofday(&end_time, NULL);
    int time_use = (end_time.tv_sec - start_time.tv_sec); // * 1000000 + (end_time.tv_usec - start_time.tv_usec);
    printf("time_use %d s\n", time_use);
    printf("\n");

    fclose(fp);
    return 0;
}