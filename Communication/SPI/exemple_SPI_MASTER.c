#ifdef MASTER

#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "SPI_MASTER.h"

void main(void) {
  // Initialisation du microcontrôleur:
  CONFIG_init();

  // Initialisation et activation de la SPI:
  SPI_init();

  // Message de démarrage:
  SPI_send("DEMARRAGE\n\r");

  while (1) {
    SPI_update();
  }
}

#endif