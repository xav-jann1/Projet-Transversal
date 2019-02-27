#include "CONFIG_8051.h"
#include "UART0.h"
#include "c8051F020.h"

void main(void) {
  // Dévalidation du watchdog:
  CONFIG_watchdog();

  // Utilisation de l'horloge externe:
  CONFIG_externClock();

  // Initialisation et activation de l'UART0:
  UART0_init();

  // Activation des interrutions:
  CONFIG_activeInterrupts();

  // Activation du crossbar:
  XBR2 = 1 << 6; // (à vérifier si c'est utile)

  // Message de démarrage:
  UART0_send("DEMARRAGE\n\r");

  while (1) {
    UART0_update();
  }
}