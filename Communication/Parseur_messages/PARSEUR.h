#ifndef PARSEUR_H
#define PARSEUR_H

// Structure d'un message interprété:
typedef struct {
  char code[4];
  int nbParams;
  char params[4];
  char valeurs[4][5];
} Commande;

/**
 * Sépare les éléments d'un message
 * @param {char*} message : message à interpréter
 */
Commande PARSE_message(char* message);


#endif  // PARSEUR_H