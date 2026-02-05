# Hello Socket

## Pourquoi select() ?

**select()** permet de surveiller plusieurs descripteurs de fichiers (sockets, pipes, etc.) simultanement et d'attendre qu'un ou plusieurs deviennent "prets":

- **Pret en lecture**: des donnees arrivent ou la connexion est fermee
- **Pret en ecriture**: le buffer d'envoi a de la place
- **Exception**: erreur ou evenement urgent
