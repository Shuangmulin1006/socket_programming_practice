#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8080
int main(int argc, char *argv[])
{
    int socket_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int ret;
    int addr_len;
    int recv_len;
    unsigned char recv_buffer[1000] = {0};
    const char *hello = "Roger that";

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    ret = bind(socket_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
    if (ret == -1) {
        perror("bind");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    while (1) {
        addr_len = sizeof(struct sockaddr);
        recv_len = recvfrom(socket_fd, recv_buffer, 999, 0, (struct sockaddr *) &client_addr, &addr_len);
        if (recv_len <= 0) {
            perror("recvfrom");
            close(socket_fd);
            exit(EXIT_FAILURE);
        } else {
            recv_buffer[recv_len] = '\0';
            printf("Get msg from client%s: %s\n", inet_ntoa(client_addr.sin_addr), recv_buffer);
            sendto(socket_fd, (const char *)hello, strlen(hello), 
                MSG_CONFIRM, (const struct sockaddr *) &client_addr,
                sizeof(client_addr));
        }
    }

    close(socket_fd);
    exit(EXIT_SUCCESS);
}