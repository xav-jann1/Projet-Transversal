#include "MASTER.h"

// Configuration 8051:
#include "../Ressources/CONFIG_8051.h"
#include "../Ressources/TIME_8051.h"

// Communications:
#include "../../Communication/Parseur_messages/PARSEUR.h"
#include "../../Communication/SPI/SPI_MASTER.h"
#include "../../Communication/UART/UART0.h"
#include "../../Communication/UART/UART1.h"

// Actionneurs:
#include "../../Actionneurs/Servomoteur/SERVO.h"

// Capteurs:
#include "../../Capteurs/Courant/COURANT.h"

void main(void) {
  MASTER_init();

  TIME_wait(2000);
  print_PC("Waiting for instructions...\n\r");

  TIME_wait(1000);
  print_PC(">");
  UART0_resetColor();

  while (1) {
    UART0_update();

    // Toutes les ms:
    if (TIME_flag_ms()) {
      TIME_clear_ms_flag();

      // Mise à jour des périphériques:
      RTOS();
    }
  }
}

/**
 * Envoie un message au PC Centrale de Commande
 * @param {char*} string : message à envoyer
 */
void print_PC(char* string) { UART0_send(string); }

/**
 * Fonction à exécuter toutes les ms pour mettre à jour tous les périphériques
 */
void RTOS() {
  // Mesure de courant:
  // COURANT_update();

  // Servomoteur:
  //   Si le servo s'est bien positionné:
  if (SERVO_update()) print_PC("AS H\r\n>");

  // Télémètres:
  // INFRA_update();
  // ULTRA_update();
}

void MASTER_init() {
  // Initialisation du 8051:
  CONFIG_init();
  TIME_init();

  /**
   * Communications:
   */

  // UART0 pour Centrale de Commande:
  UART0_init();

  // UART1 pour Serializer:
  // UART1_init();

  // SPI pour Slave:
  // SPI_init();

  /**
   * Actionneurs:
   */

  // Servomoteur:
  SERVO_init();

  /**
   * Capteurs:
   */

  // Télémètre infrarouge:
  // INFRA_init();

  // Télémètre ultrason:
  // ULTRA_init();

  // Mesure de courant:
  // COURANT_init();

  print_PC("\n\rMASTER init completed\n\r");
}