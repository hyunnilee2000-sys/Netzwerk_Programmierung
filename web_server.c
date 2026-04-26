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
    serverAddr.sin_port = htons(1600); 

    // 빠른 재시작을 위한 옵션 (포트 충돌 방지)
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    listen(sock, 1);

    printf("Web Server (AJAX version) started! Open browser at: http://192.168.1.106:1600\n\n");

    struct sockaddr_in clientAddr;
    unsigned int len = sizeof(clientAddr);
    int is_busy = 0; 

    while (1) {
        int sock2 = accept(sock, (struct sockaddr *) &clientAddr, &len);
        char buffer[2048];
        memset(buffer, 0, sizeof(buffer));
        recv(sock2, buffer, sizeof(buffer)-1, 0);

        char *first_line = strtok(buffer, "\n");
        if (first_line == NULL) { close(sock2); continue; }

        char response[8192];
        memset(response, 0, sizeof(response));

        // 🚀 변경 1: /call 이나 /reset 요청이 오면 새 페이지가 아니라 '글자'만 던져줌
        if (strstr(first_line, "GET /call") != NULL) {
            is_busy = 1;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n<span style='color:red;'>Busy</span>");
        } 
        else if (strstr(first_line, "GET /reset") != NULL) {
            is_busy = 0;
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n<span style='color:green;'>Available</span>");
        } 
        else {
            // 메인 페이지 접속 시 딱 한 번만 전체 HTML을 전송
            char status_text[50];
            if (is_busy) strcpy(status_text, "<span style='color:red;'>Busy</span>");
            else strcpy(status_text, "<span style='color:green;'>Available</span>");

            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                              "<html><head><meta charset='utf-8'><title>Video Call System</title></head>"
                              "<body style='text-align:center; font-family:sans-serif;'>"
                              "<h1>Restaurant System (No Refresh)</h1>"
                              
                              ""
                              "<h2>Status: <span id='statusArea'>%s</span></h2>"
                              
                              "<video id='myVideo' width='400' height='300' autoplay playsinline style='background:black; border-radius:10px;'></video><br>"
                              "<button onclick='startCamera()' style='padding:10px; margin:10px;'>Turn On Camera</button><br><br>"
                              
                              ""
                              "<button onclick=\"sendCommand('/call')\" style='padding:15px; background:#007BFF; color:white; border:none; border-radius:5px;'>Call Staff</button> "
                              "<button onclick=\"sendCommand('/reset')\" style='padding:15px; background:#6c757d; color:white; border:none; border-radius:5px;'>Reset Status</button>"
                              
                              "<script>"
                              "function startCamera() {"
                              "  navigator.mediaDevices.getUserMedia({ video: true, audio: false })"
                              "  .then(function(stream) {"
                              "    document.getElementById('myVideo').srcObject = stream;"
                              "  })"
                              "  .catch(function(err) {"
                              "    console.log('Camera Error: ' + err.message);"
                              "  });"
                              "}"
                              
                              "/* 🚀 변경 3: 화면 이동 없이 몰래 서버와 통신하는 Fetch API 기술 */"
                              "function sendCommand(cmd) {"
                              "  fetch(cmd)"
                              "  .then(response => response.text())"
                              "  .then(data => {"
                              "    document.getElementById('statusArea').innerHTML = data;"
                              "  });"
                              "}"
                              "</script>"
                              "</body></html>", status_text);
        }

        send(sock2, response, strlen(response), 0);
        close(sock2);
    }
    close(sock);
    return 0;
}