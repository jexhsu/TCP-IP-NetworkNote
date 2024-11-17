#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	// 创建服务器socket
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sd == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sd, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
	if (clnt_sd == -1)
		error_handling("accept() error");

	// 读取客户端请求的文件名
	read_cnt = read(clnt_sd, file_name, BUF_SIZE);
	file_name[read_cnt] = 0; // 确保字符串结束
	printf("Requested file: %s\n", file_name);

	// 打开文件
	fp = fopen(file_name, "r");
	if (fp == NULL)
	{
		char *error_msg = "File not found\n";
		write(clnt_sd, error_msg, strlen(error_msg));
		close(clnt_sd);
		close(serv_sd);
		return 1;
	}

	// 读取并发送文件内容
	while ((read_cnt = fread(buf, 1, BUF_SIZE, fp)) > 0)
	{
		write(clnt_sd, buf, read_cnt);
	}

	printf("File content sent successfully\n");

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}