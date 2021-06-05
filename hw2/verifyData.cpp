#include "hw2.h"
int main()
{
    FILE *pFile;
    int buffer[SIZE];

    if ((pFile = fopen("bin/test.bin", "rb")) == NULL)
    {
        printf("cant open the file");
        exit(0);
    }
    fread(buffer, sizeof(int), SIZE, pFile);
    std::vector<int> sorted(buffer, buffer + SIZE);
    std::sort(sorted.begin(), sorted.end());
    for (int i = 0; i < SIZE; i++)
    {
        if (sorted[i] != buffer[i])
        {
            printf("%d %d %d error!\n", i, sorted[i], buffer[i]);
            break;
        }
    }
    fclose(pFile);
    return 0;
}
