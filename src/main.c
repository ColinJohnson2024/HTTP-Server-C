#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main() {
    printf("Creating socket...\n");

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    printf("Socket created successfully! File descriptor: %d\n", server_fd);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }
    
    printf("Socket bound to port 8080\n");

    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port 8080...\n");

    printf("Waiting for connections...\n");
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        printf("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected! Client socket: %d\n", client_fd);

    char buffer[1024];
    int bytes_read = read(client_fd, buffer, sizeof(buffer) -1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Received:\n%s\n", buffer);
        }

    char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 48\r\n"
        "\r\n"
        "<html><body><h1>Hello World!</h1></body></html>";
    
    write(client_fd, response, strlen(response));
    printf("Response sent!\n");

    usleep(100000);
    close(client_fd);
    close(server_fd);
    return 0;
}
