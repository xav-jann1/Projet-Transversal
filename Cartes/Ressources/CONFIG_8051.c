#include "CONFIG_8051.h"
#include "c8051F020.h"

// Dévalidation du watchdog:
void CONFIG_watchdog() {
  WDTCN = 0xDE;
  WDTCN = 0xAD;
}

// Configure et utlise l'oscillateur externe pour SYSCLK:
int n, cp;
void CONFIG_externClock() {
  // Active l'oscillateur externe de 22Mhz:
  OSCXCN = 0x67;

  // Attend 2 ms:
  for (n = 0; n < 2; n++)
    for (cp = 0; cp < 2000; cp++)
      ;

  // Utilise l'oscillateur externe comme SYSCLK:
  OSCICN = 1 << 3;
}

// Autorise les interruptions:
void CONFIG_activeInterrupts() { EA = 1; }

// Active le Crossbar:
void CONFIG_activeCrossbar() { XBR2 |= 1 << 6; }

void CONFIG_init() {
  // interrupts
  // crossbar
}
