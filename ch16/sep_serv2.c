#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    FILE *readFP;
    FILE *writeFP;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE] = {
        0,
    };
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    listen(serv_sock, 5);
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

    readFP = fdopen(clnt_sock, "r");
    writeFP = fdopen(dup(clnt_sock), "w"); // 复制文件描述符

    fputs("FROM SERVER: Hi~ client? \n", writeFP);
    fputs("I love all of the world \n", writeFP);
    fputs("You are awesome! \n", writeFP);
    fflush(writeFP);

    shutdown(fileno(writeFP), SHUT_WR); // 对 fileno 产生的文件描述符使用 shutdown 进入半关闭状态

    fclose(writeFP);
    fgets(buf, sizeof(buf), readFP);
    fputs(buf, stdout);
    fclose(readFP);
    return 0;
}
