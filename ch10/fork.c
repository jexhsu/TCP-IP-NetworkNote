#include <stdio.h>
#include <unistd.h>

int globalVal = 10;
int main(int argc, char *argv[])
{
    pid_t pid;
    int localVal = 20;
    globalVal++, localVal += 5;

    pid = fork();
    if (pid == 0)
        globalVal += 2, localVal += 2;
    else
        globalVal -= 2, localVal -= 2;

    if (pid == 0)
        printf("Child Proc: [%d,%d] \n", globalVal, localVal);
    else
        printf("Parent Proc: [%d,%d] \n", globalVal, localVal);

    return 0;
}
