# Hello Socket

## Pourquoi select() ?

**select()** permet de surveiller plusieurs descripteurs de fichiers (sockets, pipes, etc.) simultanément et d'attendre qu'un ou plusieurs deviennent "prêts" :

- **Prêt en lecture** : des données arrivent ou la connexion est fermée
- **Prêt en écriture** : le buffer d'envoi a de la place
- **Exception** : erreur ou événement urgent

## Unix Sockets

**Unix sockets** (AF_UNIX) : communication inter-processus sur la même machine (plus rapide, pas de stack réseau)

## Prototype de **select()**

```c
#include <sys/select.h>

int select(int nfds,
           fd_set *readfds,
           fd_set *writefds,
           fd_set *exceptfds,
           struct timeval *timeout);
```

**Macros essentielles** :

```c
FD_ZERO(fd_set *set)      // Initialise le set
FD_SET(int fd, fd_set *set)   // Ajoute un fd
FD_CLR(int fd, fd_set *set)   // Retire un fd
FD_ISSET(int fd, fd_set *set) // Teste si fd est prêt
```

## Etapes

- [x] Initialisation tableau clients
- [x] Création socket serveur
- [x] Configuration adresse
- [x] Bind & Listen
- [x] Initialisation du set principal
- [ ] Boucle principale:
  - [ ] Attente bloquante d'activité
  - [ ] 1. Nouvelle connexion ? => Ajout du nouveau client
  - [ ] 2. Données provenant de clients existants
- [x] Cleanup
