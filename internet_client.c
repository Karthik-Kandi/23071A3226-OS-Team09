#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char username[50];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    printf("✅ Connected to server!\n");

    // Send username to server
    printf("👤 Enter your username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove newline
    write(client_fd, username, strlen(username));

    while (1) {
        printf("📝 You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(client_fd, buffer, strlen(buffer));

        if (strcmp(buffer, "exit\n") == 0) break;

        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        printf("\n💬 Server: %s", buffer);
    }

    printf("🚪 Disconnected from server.\n");
    close(client_fd);
    return 0;
}
