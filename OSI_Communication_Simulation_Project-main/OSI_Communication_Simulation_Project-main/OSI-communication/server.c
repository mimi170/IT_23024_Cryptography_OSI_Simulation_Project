
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int unwrap(const char *src, const char *tag,
           char *dest, size_t capacity) {
    char prefix[20];

    snprintf(prefix, sizeof(prefix), "%s{", tag);

    size_t p = strlen(prefix);
    size_t len = strlen(src);

    if (len < p + 1 || strncmp(src, prefix, p) != 0)
        return 0;

    if (src[len - 1] != '}')
        return 0;

    size_t data_len = len - p - 1;

    if (data_len >= capacity)
        return 0;

    memcpy(dest, src + p, data_len);
    dest[data_len] = '\0';

    return 1;
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr;

    char received[BUFFER_SIZE] = {0};
    char data[BUFFER_SIZE] = {0};
    char next[BUFFER_SIZE] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("=== OSI SERVER STARTED ===\n");
    printf("Waiting for client on port %d...\n", PORT);

    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == INVALID_SOCKET) {
        printf("Client connection failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("\nClient connected successfully!\n");

    int total = 0;
    int n;

    while ((n = recv(client_socket, received + total,
                     BUFFER_SIZE - 1 - total, 0)) > 0) {
        total += n;

        if (total >= BUFFER_SIZE - 1)
            break;
    }

    received[total] = '\0';

    if (n == SOCKET_ERROR) {
        printf("Receive failed.\n");
    } else {
        printf("\n=== RECEIVING DATA ===\n");

        printf("[Physical (RX)] Data: %s\n", received);

        if (!unwrap(received, "PHY", data, sizeof(data)))
            goto invalid;

        printf("[DataLink (RX)] Data: %s\n", data);

        if (!unwrap(data, "MAC", next, sizeof(next)))
            goto invalid;
        strcpy(data, next);

        printf("[Network (RX)] Data: %s\n", data);

        if (!unwrap(data, "IP", next, sizeof(next)))
            goto invalid;
        strcpy(data, next);

        printf("[Transport (RX)] Data: %s\n", data);

        if (!unwrap(data, "TCP", next, sizeof(next)))
            goto invalid;
        strcpy(data, next);

        printf("[Session (RX)] Data: %s\n", data);

        if (!unwrap(data, "SES", next, sizeof(next)))
            goto invalid;
        strcpy(data, next);

        printf("[Presentation (RX)] Data: %s\n", data);

        if (!unwrap(data, "PRS", next, sizeof(next)))
            goto invalid;
        strcpy(data, next);

        printf("[Application (RX)] Data: %s\n", data);

        if (!unwrap(data, "APP", next, sizeof(next)))
            goto invalid;

        printf("\nFinal message: %s\n", next);
        printf("=== MESSAGE DELIVERED SUCCESSFULLY ===\n");
        goto cleanup;

        invalid:
        printf("\nInvalid packet format!\n");
    }

    cleanup:
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
