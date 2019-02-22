#include <stdio.h>
#include <string.h>
#include "PARSEUR.h"

void main() {
  // Message à interpréter:
  char* message = "SD F:50 P:63 W:52 B:12";
  printf("Message interprété : '%s'\n", message);

  // Interprétation du texte:
  Commande commande = PARSE_message(message);

  // Affichage du contenue de la commande:
  printf("code:     '%s'\n", commande.code);
  printf("nbParams: '%d'\n", commande.nbParams);
  for (int i = 0; i < commande.nbParams; i++) {
    printf("param: '%c', valeur: '%s'", commande.params[i], commande.valeurs[i]);
    if (commande.valeurs[i][0] == '\0') printf("NULL");
    printf("\n");
  }
}