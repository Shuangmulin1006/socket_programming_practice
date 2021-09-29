#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
    int addr_len;
    int ret;
	struct sockaddr_in serv_addr, client_addr;
	char buffer[1024] = "hello";
    char recv_buffer[1024] = {0};
    char *hello = "Hello from client";
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
        perror("socket creation failed");
        exit(EXIT_FAILURE);
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&client_addr, 0 , sizeof(client_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int recv_len;
    while (1) {
		if (fgets(buffer, 1023, stdin)) {
            printf("input: %s\n", buffer);
            sendto(sock, (const char *)buffer, strlen(buffer),
                MSG_CONFIRM, (const struct sockaddr *) &serv_addr, 
                    sizeof(serv_addr));
            printf("Send over\n");
            
            recv_len = recvfrom(sock, recv_buffer, 1023, 0, (struct sockaddr *) &client_addr, sizeof(struct sockaddr));
            printf("From server: %s\n", recv_buffer);

		}
	}
	close(sock);
	return 0;
}