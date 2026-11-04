#include <stdio.h>
#include "layers.h"


int main() {
char msg[] = "Hello OSI Model";
printf("\n=== TRANSMISSION START ===\n\n");
Application_Send(msg);
return 0;
}
