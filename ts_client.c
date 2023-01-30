#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUMBER 50000
#define BUFFER_SIZE 1024

int main() {
    int socket_desc;
struct sockaddr_in server_address;
char buffer[BUFFER_SIZE];


// create a socket
socket_desc = socket(AF_INET, SOCK_STREAM, 0);
if (socket_desc < 0) {
    perror("Error creating socket");
    exit(1);
}

// setup server address
server_address.sin_family = AF_INET;
server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
server_address.sin_port = htons(PORT_NUMBER);

// connect to the server
if (connect(socket_desc, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    perror("Error connecting to server");
    exit(1);
}

// receive the file and save it to disk
FILE *file = fopen("transport.ts", "w");
if (file == NULL) {
    perror("Error creating file");
    exit(1);
}
int bytes_received;
while ((bytes_received = recv(socket_desc, buffer, BUFFER_SIZE, 0)) > 0) {
    fwrite(buffer, 1, bytes_received, file);
}
fclose(file);
printf("ts file received successfully\n");

// close the socket
close(socket_desc);
return 0;
}
