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
int arr[SIZE];
int threadNow = 0;
int allCount = 0;
int num = 0;
int finish = 0;
sem_t amount;
sem_t threadSize;
sem_t finished;
pthread_mutex_t mutex;
pthread_mutex_t Nsorted;

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
void *sort_thread(void *data);

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

std::vector<pos> posStat;
int main(int argc, char *argv[])
{
    //init
    int *data;
    int fd = open("bin/test.bin", O_RDWR);
    if (fd < 0)
    {
        printf("open failed!\n");
        return -1;
    }
    data = (int *)mmap(NULL, SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (!data)
    {
        printf("mmap failed\n");
        close(fd);
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&Nsorted, NULL);
    srand(time(NULL));
    sem_init(&finished, 0, 0);
    sem_init(&threadSize, 0, 1);
    std::nth_element(data, data + SIZE / 2, data + SIZE);

    posStat.push_back({0, SIZE / 2});
    posStat.push_back({SIZE / 2, SIZE});

    std::vector<int> sorted(SIZE);
    std::copy(data, data + SIZE, sorted.begin());
    std::sort(sorted.begin(), sorted.end());
    pthread_t *thread_handles;
    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));

    for (long thread = 0; thread < thread_count; ++thread)
    {
        pthread_create(&thread_handles[thread], NULL,
                       sort_thread, data);
    }
    //wait finish
    sem_wait(&finished);

    //check
    for (int i = 0; i < SIZE; i++)
    {
        if (data[i] != sorted[i])
        {
            printf("error!\n");
            break;
        }
    }

    //clear
    free(thread_handles);
    msync(data, SIZE * sizeof(int), MAP_SHARED);
    munmap(data, SIZE);
    // free(data);
    sem_destroy(&threadSize);
    sem_destroy(&finished);
    close(fd);
    shm_unlink("qsort");

    return 0;
}

void *sort_thread(void *data)
{
    int *arr = (int *)data;
    while (finish != 1 || posStat.size() > 0)
    {
        pthread_mutex_lock(&mutex);
        pos tmp;
        if (posStat.size() != 0)
        {
            tmp = posStat.front();
            std::vector<pos>::iterator iter = find(posStat.begin(), posStat.end(), tmp);
            if (iter != posStat.end())
                posStat.erase(iter);
        }
        else
            tmp = {0, 0};

        int start = tmp.min;
        int end = tmp.max;
        int stride = end - start;
        pthread_mutex_unlock(&mutex);

        if (stride > DIVIDE)
        {
            std::nth_element(arr + start, arr + start + stride / 2, arr + end);
            pthread_mutex_lock(&mutex);
            if (stride > DIVIDE)
            {
                posStat.push_back({start, start + stride / 2});
                posStat.push_back({start + stride / 2, end});
            }
            pthread_mutex_unlock(&mutex);
            if (threadNow < MAXTHREAD)
            {
                sem_wait(&threadSize);
                threadNow = threadNow + 2;
                sem_post(&threadSize);
                pthread_t *thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
                for (long thread = 0; thread < thread_count; ++thread)
                {
                    pthread_create(&thread_handles[thread], NULL,
                                   sort_thread, data);
                }
            }
        }
        else
        {
            if (stride != 0)
                std::qsort(arr + start, stride, sizeof(int), compare);
            pthread_mutex_lock(&Nsorted);
            allCount += stride;
            if (allCount == SIZE)
            {
                finish = 1;
                sem_post(&finished);
            }
            pthread_mutex_unlock(&Nsorted);
        }
    }
    pthread_exit(0);
}