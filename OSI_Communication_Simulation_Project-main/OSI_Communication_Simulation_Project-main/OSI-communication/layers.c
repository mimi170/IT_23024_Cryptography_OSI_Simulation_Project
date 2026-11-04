#include <stdio.h>
#include <string.h>
#include "layers.h"

// Utility wrapper to print transitions
void log_layer(const char *layer, const char *data) {
printf("[%s] Data: %s\n", layer, data);
}

// -------------------- TRANSMIT SIDE --------------------
void Application_Send(const char *msg) {
log_layer("Application", msg);
char buffer[256]; sprintf(buffer, "APP{%s}", msg);
Presentation_Send(buffer);
}

void Presentation_Send(const char *data) {
log_layer("Presentation", data);
char buffer[256]; sprintf(buffer, "PRS{%s}", data);
Session_Send(buffer);
}

void Session_Send(const char *data) {
log_layer("Session", data);
char buffer[256]; sprintf(buffer, "SES{%s}", data);
Transport_Send(buffer);
}


void Transport_Send(const char *data) {
log_layer("Transport", data);
char buffer[256]; sprintf(buffer, "TCP{%s}", data);
Network_Send(buffer);
}


void Network_Send(const char *data) {
log_layer("Network", data);
char buffer[256]; sprintf(buffer, "IP{%s}", data);
DataLink_Send(buffer);
}


void DataLink_Send(const char *data) {
log_layer("DataLink", data);
char buffer[256]; sprintf(buffer, "MAC{%s}", data);
Physical_Send(buffer);
}


void Physical_Send(const char *data) {
log_layer("Physical", data);
printf("\n=== SIGNAL SENT OVER WIRE ===\n\n");


// Trigger receive chain
Physical_Receive(data);
}

// -------------------- RECEIVE SIDE --------------------

// -------------------- RECEIVE SIDE --------------------

void unwrap_data(const char *data, int prefix_len, char *buffer) {
    strcpy(buffer, data + prefix_len);

    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '}') {
        buffer[len - 1] = '\0';
    }
}

void Physical_Receive(const char *data) {
    log_layer("Physical (RX)", data);
    char buffer[256];
    unwrap_data(data, 4, buffer);
    DataLink_Receive(buffer);
}

void DataLink_Receive(const char *data) {
    log_layer("DataLink (RX)", data);
    char buffer[256];
    unwrap_data(data, 3, buffer);
    Network_Receive(buffer);
}

void Network_Receive(const char *data) {
    log_layer("Network (RX)", data);
    char buffer[256];
    unwrap_data(data, 4, buffer);
    Transport_Receive(buffer);
}

void Transport_Receive(const char *data) {
    log_layer("Transport (RX)", data);
    char buffer[256];
    unwrap_data(data, 4, buffer);
    Session_Receive(buffer);
}

void Session_Receive(const char *data) {
    log_layer("Session (RX)", data);
    char buffer[256];
    unwrap_data(data, 4, buffer);
    Presentation_Receive(buffer);
}

void Presentation_Receive(const char *data) {
    log_layer("Presentation (RX)", data);
    char buffer[256];
    unwrap_data(data, 4, buffer);
    Application_Receive(buffer);
}

void Application_Receive(const char *data) {
    log_layer("Application (RX)", data);
    printf("\n=== FINAL MESSAGE DELIVERED TO APPLICATION ===\n");
}
