#include <string.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(void){
    printf("========== [Staff Tablet] ==========\n");
    
    while(1) {
        printf("\n[Info] Press Enter to reset status to 'Available'. (Exit: Ctrl+C)\n");
        getchar(); 

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(1500); 
        inet_aton("127.0.0.1", &serverAddr.sin_addr); 

        if (connect(sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
            printf("Cannot connect to the server. Please check if the server is running.\n");
            continue;
        }

        char* str = "RESET";
        send(sock, str, strlen(str) + 1, 0);
        
        char buffer[1000];
        memset(buffer, 0, 1000);
        recv(sock, buffer, sizeof(buffer) - 1, 0);
        printf("▶ Server Response: %s\n", buffer);

        close(sock);
    }
    return 0;
}