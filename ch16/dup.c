#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int cfd1, cfd2;
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    cfd1 = dup(1);        // 创建一个新的文件描述符 cfd1，它指向标准输出。
    cfd2 = dup2(cfd1, 7); // 将文件描述符 cfd1（指向标准输出）复制到文件描述符 7。

    printf("fd1=%d , fd2=%d \n", cfd1, cfd2);
    write(cfd1, str1, sizeof(str1));
    write(cfd2, str2, sizeof(str2));

    close(cfd1);
    close(cfd2); // 终止复制的文件描述符，但是仍有一个文件描述符
    write(1, str1, sizeof(str1));
    close(1);
    write(1, str2, sizeof(str2)); // 无法完成输出
    return 0;
}
