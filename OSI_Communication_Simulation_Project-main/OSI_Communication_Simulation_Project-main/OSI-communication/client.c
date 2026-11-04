
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

int main(void) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[512];
    char packet[BUFFER_SIZE];

    printf("=== OSI CLIENT ===\n");
    printf("Enter your message: ");

    if (fgets(message, sizeof(message), stdin) == NULL) {
        return 1;
    }

    message[strcspn(message, "\n")] = '\0';

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server,
                sizeof(server)) == SOCKET_ERROR) {
        printf("Connection failed. Start the server first.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("\n=== ENCAPSULATION (SENDER) ===\n");

    snprintf(packet, sizeof(packet), "APP{%s}", message);
    printf("[Application] %s\n", packet);

    char temp[BUFFER_SIZE];

    snprintf(temp, sizeof(temp), "PRS{%s}", packet);
    strcpy(packet, temp);
    printf("[Presentation] %s\n", packet);

    snprintf(temp, sizeof(temp), "SES{%s}", packet);
    strcpy(packet, temp);
    printf("[Session] %s\n", packet);

    snprintf(temp, sizeof(temp), "TCP{%s}", packet);
    strcpy(packet, temp);
    printf("[Transport] %s\n", packet);

    snprintf(temp, sizeof(temp), "IP{%s}", packet);
    strcpy(packet, temp);
    printf("[Network] %s\n", packet);

    snprintf(temp, sizeof(temp), "MAC{%s}", packet);
    strcpy(packet, temp);
    printf("[Data Link] %s\n", packet);

    snprintf(temp, sizeof(temp), "PHY{%s}", packet);
    strcpy(packet, temp);
    printf("[Physical] %s\n", packet);

    printf("\n=== SENDING THROUGH TCP SOCKET ===\n");

    int len = (int)strlen(packet);
    int sent = 0;

    while (sent < len) {
        int n = send(sock, packet + sent, len - sent, 0);

        if (n == SOCKET_ERROR) {
            printf("Send failed.\n");
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        sent += n;
    }

    shutdown(sock, SD_SEND);

    printf("Message sent successfully!\n");

    closesocket(sock);
    WSACleanup();

    return 0;
}
