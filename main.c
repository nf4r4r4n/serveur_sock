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
    read_set = master_set;
    activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);

    if (activity < 0) {
      perror("Select error");
      exit(EXIT_FAILURE);
    }

    // 1. Nouvelle connexion
    if (FD_ISSET(server_fd, &read_set)) {
      client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
      if (client_fd < 0) {
        perror("Accept failed");
        continue;
      }

      // Trouver un slot libre pour le nouveau client
      int slot = -1;
      for (i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == -1) {
          slot = 1;
          break;
        }
      }

      if (slot == -1) {
        // Trop de clients
        printf("Trop de clients");
        close(client_fd);
      } else {
        // Ajouter le client
        client_sockets[slot] = client_fd;
        FD_SET(client_fd, &master_set);
        if (client_fd > max_fd)
          max_fd = client_fd;

        printf("Client connecte [%d]\n", slot);
      }
    }

    // 2. Donnees des clients existants
    for (i = 0; i < MAX_CLIENTS; i++) {
      int client_val = client_sockets[i];
      if (client_val > 0 && FD_ISSET(client_val, &read_set)) {
        // Reception des donnees
        if ((valread = recv(client_val, buffer, BUFFER_SIZE, 0)) == 0) {
          // Deconnexion propre ou connexion perdue
          close(client_val);
          FD_CLR(client_val, &master_set);
          client_sockets[i] = -1;
        } else {
          // Traitement du message
          buffer[valread] = '\0';

          // Supprimer \r\n eventuels (netcat)
          buffer[strcspn(buffer, "\r\n")] = '\0';

          // Traitement de tout type de message
        }
      }
    }
  }

  // Cleanup
  close(server_fd);
  return (0);
}