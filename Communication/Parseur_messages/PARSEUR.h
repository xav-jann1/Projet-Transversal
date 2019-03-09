#ifndef PARSEUR_H
#define PARSEUR_H

// Structure d'un message interprété:
typedef struct {
  char cmd[4];
  char nbParams;
  char params[4];
  char valeurs[4][5];
} Commande;

/**
 * Sépare les éléments d'un message
 * @param {char*} message : message à interpréter
 */
Commande PARSEUR_message(char* message);

#endif  // PARSEUR_H