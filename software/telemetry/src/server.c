#define _WIN32_WINNT 0x0601   // Windows 7+

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5000
#define BUFFER_SIZE 1024

int main(void) {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    /* Initialize Winsock */
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        printf("socket failed\n");
        WSACleanup();
        return 1;
    }

    /* Allow reuse of address */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               (const char *)&opt, sizeof(opt));

    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /* Bind */
    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        printf("bind failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    /* Listen */
    if (listen(server_fd, 5) == SOCKET_ERROR) {
        printf("listen failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    /* Accept connection */
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &client_len);
    if (client_fd == INVALID_SOCKET) {
        printf("accept failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Client connected: %s\n",
           inet_ntoa(client_addr.sin_addr));

    /* Receive data */
    int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        printf("Received: %s\n", buffer);

        /* Send response */
        const char *reply = "Hello from server\n";
        send(client_fd, reply, (int)strlen(reply), 0);
    }

    /* Cleanup */
    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}