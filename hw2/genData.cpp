#include "hw2.h"

#define SIZE 1000000
int main()
{
    FILE *pFile;
    int buffer[SIZE];
    if ((pFile = fopen("bin/test.bin", "wb")) == NULL)
    {
        printf("cant open the file");
        exit(0);
    }
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        buffer[i] = rand() % SIZE;

    fwrite(buffer, sizeof(int), SIZE, pFile);
    fclose(pFile);
    return 0;
}
