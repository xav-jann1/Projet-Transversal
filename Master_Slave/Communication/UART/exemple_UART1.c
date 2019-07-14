#include "CONFIG_8051.h"
#include "UART1.h"
#include "c8051F020.h"

void main(void) {
  // Initialisation du microcontrôleur:
  CONFIG_init();

  // Initialisation et activation de l'UART0:
  UART1_init();

  UART1_send("DEMARRAGE\n\r");

  while (1) {
    UART1_update();
  }
}