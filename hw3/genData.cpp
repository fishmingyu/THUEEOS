#include "hw3.h"
int main()
{
    FILE *pFile;
    char *buffer = (char *)malloc(SIZE * sizeof(char));
    if ((pFile = fopen("bin/test.bin", "wb")) == NULL)
    {
        printf("cant open the file");
        exit(0);
    }
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        buffer[i] = '0' + rand() % 43;

    fwrite(buffer, sizeof(char), SIZE, pFile);
    fclose(pFile);
    return 0;
}
