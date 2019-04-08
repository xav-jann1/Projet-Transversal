#include "SLAVE.h"
#include <stdio.h>

// Configuration 8051:
#include "../Ressources/CONFIG_8051.h"
#include "../Ressources/TIME_8051.h"

// Communications:
#include "../../Communication/Parseur_messages/PARSEUR.h"
#include "../../Communication/SPI/SPI_SLAVE.h"
#include "../../Communication/UART/UART0.h"

// Actionneurs:
#include "../../Actionneurs/Pointeur_lumineux/POINTEUR.h"
#include "../../Actionneurs/Servomoteur/SERVO.h"

// Capteurs:
//#include "../../Capteurs/Camera/CAMERA.h"

/**
 * Fonction principale
 */
void main(void) {
  // Initialisation de la carte et des périphériques:
  SLAVE_init();

  // Messages de démarrage:
  print_MASTER("\n\rSLAVE init completed\n\r");
  TIME_wait(2000);
  print_MASTER("Waiting for instructions...\n\r");
  TIME_wait(1000);
  print_MASTER(">");
  //UART0_resetColor();

  // Boucle principale:
  while (1) {
    SPI_update();
    //UART0_update();

    // Toutes les ms:
    if (TIME_flag_ms()) {
      TIME_clear_ms_flag();

      // Mise à jour des périphériques:
      RTOS();
    }
  }
}

/**
 * Envoie un message à la carte Master
 * @param {char*} string : message à envoyer
 */
void print_MASTER(char* string) { SPI_send(string); }

/**
 * Fonction à exécuter toutes les ms pour mettre à jour tous les périphériques
 */
void RTOS() {
  // Servomoteur:
  //   Si le servo s'est bien positionné:
  if (SERVO_update()) print_MASTER("AS V\r\n>");

  // Pointeur lumineux:
  POINTEUR_update();
}

/**
 * Initialisation de tous les périphériques
 */
void SLAVE_init() {
  // Initialisation du 8051:
  CONFIG_init();
  TIME_init();

  /**
   * Communications:
   */

  // SPI pour Master:
  SPI_init();

  // UART0 pour STM32:
  // UART0_init();

  /**
   * Actionneurs:
   */

  // Servomoteur:
  SERVO_init();

  // Pointeur lumineux:
  POINTEUR_init();

  /**
   * Capteurs:
   */

  // Prise de vue:
  // CAMERA_init();
}

/**
 * Arrêt d'urgence:
 *  Arrêt complet de la base roulante, les opérations en cours sont
 *  interrompues, les données en cours d’acquisition sont perdues
 */
void SLAVE_exit() { POINTEUR_stop(); }