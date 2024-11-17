#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
  int sd;
  char buf[BUF_SIZE];
  char file_name[BUF_SIZE];
  int read_cnt;
  struct sockaddr_in serv_adr;

  if (argc != 3)
  {
    printf("Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  // 创建socket
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd == -1)
    error_handling("socket() error");

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  if (connect(sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    error_handling("connect() error");

  // 输入并发送文件名
  printf("Input file name: ");
  scanf("%s", file_name);
  write(sd, file_name, strlen(file_name));

  // 接收并打印文件内容
  printf("\nFile content:\n");
  printf("----------------------------------------\n");
  while ((read_cnt = read(sd, buf, BUF_SIZE)) > 0)
  {
    buf[read_cnt] = 0; // 确保字符串结束
    printf("%s", buf);
  }
  printf("\n----------------------------------------\n");

  close(sd);
  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}