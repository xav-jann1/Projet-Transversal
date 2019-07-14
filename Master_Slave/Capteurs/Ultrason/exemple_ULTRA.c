#include "c8051F020.h"
#include "ULTRA.h"
#include "../../Projet/Cartes/Ressources/CONFIG_8051.h"
#include "../../Projet/Cartes/Ressources/TIME_8051.h"

void main() {
  CONFIG_init();
  TIME_init();
  
  ULTRA_init();
  ULTRA_mesure_avant();
  //ULTRA_mesure_arriere();
  
  while(1) {
    
    // Si mesure réalisée:
    if(ULTRA_update()) {
      int mesure = ULTRA_getMesure();

      char* string;
      sprintf(string, "KOB XX:%d\r\n>", mesure);
      UART0_send(string);
    }
    TIME_wait(1000);
  }
}