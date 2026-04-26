#include <string.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(void){
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serverAddr.sin_port = htons(1500); 

    bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    listen(sock, 1);

    printf("Server started. Waiting for client connections...\n\n");

    struct sockaddr_in clientAddr;
    unsigned int len = sizeof(clientAddr);

    // Staff status variable (0: Available/Idle, 1: Busy)
    int is_busy = 0; 

    while(1) {
        int sock2 = 0;
        char buffer[1000];
        memset(buffer, 0, sizeof(buffer));

        sock2 = accept(sock, (struct sockaddr *) &clientAddr, &len);
        recv(sock2, buffer, sizeof(buffer) - 1, 0);

        char *clientAddrIP = inet_ntoa(clientAddr.sin_addr);
        printf("[%s] Received message: %s\n", clientAddrIP, buffer);

        // Process commands based on status
        if (strncmp(buffer, "CALL_KITCHEN", 12) == 0) {
            
            if (is_busy == 1) {
                printf("--> [Status] Staff is busy. Sending notification to customer.\n");
                char* reply = "NOTIFICATION: The staff is currently busy. Please wait.";
                send(sock2, reply, strlen(reply) + 1, 0);
            } else {
                printf("--> [Status] Call accepted! Changing staff status to 'Busy'.\n");
                is_busy = 1; 
                char* reply = "ACK_KITCHEN_CALLED: Call received successfully.";
                send(sock2, reply, strlen(reply) + 1, 0);
            }

        } else if (strncmp(buffer, "RESET", 5) == 0) {
            printf("--> [Status] Staff status reset to 'Available'.\n");
            is_busy = 0;
            char* reply = "STATUS_RESET_OK";
            send(sock2, reply, strlen(reply) + 1, 0);

        } else {
            printf("--> [Warning] Unknown command received.\n");
            char* reply = "ERROR_UNKNOWN_COMMAND";
            send(sock2, reply, strlen(reply) + 1, 0);
        }
        
        close(sock2);
        printf("Client disconnected. Waiting for next connection...\n---------------------------\n");
    }

    close(sock);
    return 0;
}