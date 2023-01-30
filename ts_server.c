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
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_address_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    // create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // bind the socket to a port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT_NUMBER);
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket to port");
        exit(1);
    }

    // listen for incoming connections
    listen(server_socket, 5);

    // accept an incoming connection
    client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len);
    if (client_socket < 0) {
        perror("Error accepting connection");
        exit(1);
    }
    printf("Connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // read the file and send it to the client
    FILE *file = fopen("ocean.ts", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }
    fclose(file);
    printf("ts file sent successfully\n");

    // close the sockets
    close(client_socket);
    close(server_socket);
    return 0;
}
