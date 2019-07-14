#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "UART0.h"

void main(void) {
  // Initialisation du microcontrôleur:
  CONFIG_init();

  // Initialisation et activation de l'UART0:
  UART0_init();

  // Message de démarrage:
  UART0_send("DEMARRAGE\n\r");

  while (1) {
    UART0_update();
  }
}