#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 30

int main() {
  int server_fd;
  int client_fd;
  int max_fd;
  int activity;
  int valread;
  struct sockaddr_in address;
  struct sockaddr_in client_addr;
  socklen_t addrlen = sizeof(client_addr);
  fd_set master_set;
  fd_set read_set;
  int client_sockets[MAX_CLIENTS];
  char buffer[BUFFER_SIZE];
  int i;

  // Initialisation tableau clients
  for (i = 0; i < MAX_CLIENTS; i++)
    client_sockets[i] = -1;

  // Creation socket serveur
  if ((server_fd == socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // Configuration adresse serveur
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind & Listen
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 5) < 0 ) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  // Initialisation du set principal de descripteurs
  FD_ZERO(&master_set);
  FD_SET(server_fd, &master_set);
  max_fd = server_fd;

  // Boucle principale
  while (1) {

  }

  // Cleanup
  close(server_fd);
  return (0);
}