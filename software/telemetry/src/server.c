#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // 1. Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Optional: Forcefully attach socket to the port 8080, useful for
    // avoiding "Address already in use" errors on frequent restarts.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind the socket to port 8080 and any network interface
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    address.sin_port = htons(PORT);       // Convert port to network byte order

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) == -1) { // Max 3 pending connections in queue
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // 4. Accept a client connection (this is a blocking call)
    printf("Server listening on port %d\n", PORT);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted\n");

    // 5. Read data from client and send a response
    valread = read(new_socket, buffer, 1024);
    printf("Client message: %s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // 6. Close the connection and listening socket
    close(new_socket);
    close(server_fd); // In a real server, you'd keep this open in a loop

    return 0;
}