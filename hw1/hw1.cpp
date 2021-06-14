#include "hw1.h"

void *customer_func(void *c)
{
    Customer &customer = *(Customer *)c;
    sleep(customer.enter);      // enter time
    pthread_mutex_lock(&mutex); // protect queue
    wait_queue.push(customer);
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_customer);
    sem_wait(&sem_counter); // ready to be served
    sleep(customer.wait);   // serve time
    pthread_exit(NULL);
    return NULL;
}

void *counter_func(void *c)
{
    Counter &counter = *(Counter *)c;
    while (!finish)
    {
        int tryw = sem_trywait(&sem_customer);
        int wait_time = 0;
        if (tryw == 0)
        {
            pthread_mutex_lock(&mutex);
            if (!wait_queue.empty())
            {
                Customer &customer = customer_all[wait_queue.front().id - 1];
                wait_queue.pop();
                customer.counter = counter.id;
                wait_time = customer.wait;
                customer.begin = time(NULL) - start_sim;
                finish_num++;
            }
            pthread_mutex_unlock(&mutex);
            sem_post(&sem_counter);
            sleep(wait_time); // simulate serve
            pthread_mutex_lock(&mutex);

            if (finish_num >= customer_num)
            {
                finish = 1;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_mutex_lock(&mutex);
    finish_counter++;
    pthread_mutex_unlock(&mutex);
    if (finish_counter == counter_all.size())
        sem_post(&counter_exit);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char **argv)
{
    counter_num = atoi(argv[1]); // receive from arg
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_counter, 0, 0);
    sem_init(&sem_customer, 0, 0);
    sem_init(&counter_exit, 0, 0);
    load_data("input.txt");
    start_sim = time(NULL);
    for (int i = 0; i < counter_num; ++i)
    {
        counter_all.push_back(0);
        counter_all[i].id = i + 1;
        int error = pthread_create(&(counter_all[i].tid), NULL, counter_func, (void *)(&counter_all[i]));
        if (error)
        {
            perror("pthread_creat failed!");
            exit(-1);
        }
    }

    for (int i = 0; i < customer_all.size(); ++i)
    {
        int error = pthread_create(&(customer_all[i].tid), NULL, customer_func, (void *)(&customer_all[i]));
        if (error)
        {
            perror("pthread_creat failed!");
            exit(-1);
        }
    }
    sem_wait(&counter_exit); // wait all counter exit
    printf("counters all exit\n");
    for (int i = 0; i < customer_all.size(); ++i)
        pthread_join(customer_all[i].tid, NULL);

    for (int i = 0; i < counter_all.size(); ++i)
        pthread_join(counter_all[i].tid, NULL);

    printf("All customers have been served!\n");

    FILE *fp = fopen("output-1.txt", "w+");
    for (int i = 0; i < customer_all.size(); ++i)
        fprintf(fp, "%d %d %d %d %d\n", customer_all[i].id, customer_all[i].enter, customer_all[i].begin, customer_all[i].begin + customer_all[i].wait, customer_all[i].counter);
    fclose(fp);
    sem_destroy(&sem_counter);
    sem_destroy(&sem_customer);
    sem_destroy(&counter_exit);
    return 0;
}