#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <queue>
#include <semaphore.h>
#include <unistd.h>
#include <vector>

struct Customer
{
    pthread_t tid;
    int id;
    int counter;
    int enter; // enter time
    int begin; // serve begin time
    int wait;  // wait time during serve
    Customer(int id, int enter, int wait) : id(id), enter(enter), wait(wait) {}
};

struct Counter
{
    pthread_t tid; // thread id
    int id;        // counter id
    Counter(int id) : id(id){};
};

std::vector<Customer> customer_all;
std::vector<Counter> counter_all;
std::queue<Customer> wait_queue; // customers queue
int customer_num;
int counter_num;
int finish_num = 0;
int finish = 0;
int finish_counter = 0;
time_t start_sim;
pthread_mutex_t mutex;
sem_t sem_customer;
sem_t sem_counter;
sem_t counter_exit;

void load_data(const char *file_in)
{
    FILE *fp = fopen(file_in, "r");
    char str[60];
    int id, enter, wait;
    const char s[2] = " ";
    while (fgets(str, 60, fp))
    {
        char *token;
        token = strtok(str, s);
        id = atoi(token);
        token = strtok(NULL, s);
        enter = atoi(token);
        token = strtok(NULL, s);
        wait = atoi(token);
        customer_all.push_back(Customer(id, enter, wait));
        customer_num++;
    }
    fclose(fp);
}