#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080

int main(int argc, char *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	int client_num  = -1;
	char buffer[1024] = {0};
	const char *hello = "Roger that";

	//* Socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	//* setcockopt
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//* Bind
	if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	//* Listen
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while (1) {
		new_socket = accept(server_fd, (struct sockaddr *) &address, &addrlen);
		if (new_socket == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		client_num++;
		printf("Server get connetcion from client%d: %s\n", client_num, inet_ntoa(address.sin_addr));

		if (!fork()) {
			/* Child process */
			while (1) {
				int recv_len = recv(new_socket, buffer, 1024, 0);
				if (recv_len <= 0) {
					perror("recv");
					close(new_socket);
					exit(EXIT_FAILURE);
				} else {
					buffer[recv_len] = '\0';
					printf("Get msg from client%d: %s\n", client_num, buffer);
					send(new_socket, hello, strlen(hello), 0);
				}
		
			}
			close(new_socket);
		}
	}
	close(server_fd);




	// if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t*) &addrlen)) < 0) {
	// 	perror("accept");
	// 	exit(EXIT_FAILURE);
	// }
	// valread = recv(new_socket, buffer, 1024, 0);
	// printf("%s\n", buffer);
	// send(new_socket, hello, strlen(hello), 0);
	// printf("Hello message sent\n");
	return 0;
}
