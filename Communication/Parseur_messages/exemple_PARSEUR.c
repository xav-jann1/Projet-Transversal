#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PARSEUR.h"

#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "../UART/UART0.h"

void test() {
  int i, nbParams;
  char buff[8];

  // Message à interpréter:
  char* message = "C 128 B D:145 A:255";

  // Interprétation du texte:
  Commande commande;
  commande = PARSEUR_message(message);

  // printf("Message interprété : '%s'\n", message);
  UART0_send3("Message interprété : '", message, "'\n\r");

  // Affichage du contenu de la commande:
  // printf("code:     '%s'\n", commande.code);
  UART0_send3("code:     '", commande.cmd, "'\n\r");

  // printf("nbParams: '%d'\n", commande.nbParams);
  nbParams = commande.nbParams;
  sprintf(buff, "%d", nbParams);
  UART0_send3("nbParams: '", buff, "'\n\r");

  // Pour chaque paramètre de la commande:
  for (i = 0; i < commande.nbParams; i++) {
    // printf("param: '%c', valeur: '%s'", commande.params[i],
    // commande.valeurs[i]);
    sprintf(buff, "%c", commande.params[i]);
    UART0_send3("param: '", buff, "', ");

    sprintf(buff, "%d", commande.valeurs[i]);
    UART0_send3("valeur: '", buff, "'");

    if (commande.valeurs[i] == 0xFF) UART0_send("NULL");  // printf("NULL");

    UART0_send("\n\r");
  }
}

void main() {
  // Initialisation:
  CONFIG_init();
  UART0_init();

  // Test:
  test();

  while (1)
    ;
}