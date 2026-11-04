#ifndef LAYERS_H
#define LAYERS_H


void Application_Send(const char *msg);
void Presentation_Send(const char *data);
void Session_Send(const char *data);
void Transport_Send(const char *data);
void Network_Send(const char *data);
void DataLink_Send(const char *data);
void Physical_Send(const char *data);


void Physical_Receive(const char *data);
void DataLink_Receive(const char *data);
void Network_Receive(const char *data);
void Transport_Receive(const char *data);
void Session_Receive(const char *data);
void Presentation_Receive(const char *data);
void Application_Receive(const char *data);


#endif
