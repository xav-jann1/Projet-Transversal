#ifdef SLAVE
#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "../../Cartes/Ressources/c8051F020.h"
#include "SPI_SLAVE.h"

void main(void) {
  // Initialisation du microcontrôleur:
  CONFIG_init();

  // Initialisation et activation de la SPI:
  SPI_init();

  // Crossbar:
  P0MDOUT |= (1 << 1);                // Push-pull : MISO(P0.1)
  P0MDOUT &= ~((1 << 2) + (1 << 4));  // Input : MOSI(P0.2),  SEL(P0.4)

  // Message de démarrage:
  // SPI_send("DEMARRAGE\n\r");

  while (1) {
    SPI_update();
  }
}

#endif