#include "SLAVE.h"
#include <stdio.h>
#include "../../Cartes/Ressources/c8051F020.h"

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
#include "../../Capteurs/Photo/PHOTO.h"

/**
 * Fonction principale
 */
void main(void) {
  // Initialisation de la carte et des périphériques:
  SLAVE_init();

  // Messages de démarrage:
  //print_MASTER("\n\rSLAVE init completed\n\r");
  //TIME_wait(2000);
  //print_MASTER("Waiting for instructions...\n\r");
  //TIME_wait(1000);
  //print_MASTER(">");
  //UART0_resetColor();
	
  UART0_send("SD F:6 P:25 W:50 B:3\r");

  // Boucle principale:
  while (1) {
    SPI_update();
    UART0_update();

    // Toutes les ms:
    if (TIME_flag_ms()) {
      TIME_clear_ms_flag();

      // Mise à jour des périphériques:
      RTOS();
    }
		
		// Toutes les 100 ms:
    if (TIME_flag_100ms()) {
      TIME_clear_100ms_flag();

      // Mise à jour des périphériques:
      RTOS_100ms();
    }
  }
}

/**
 * Envoie un message à la carte Master
 * @param {char*} string : message à envoyer
 */
void print_MASTER(char* string) { UART0_send(string); } //SPI_send(string); }

/**
 * Fonction à exécuter toutes les ms pour mettre à jour tous les périphériques
 */
void RTOS() {
  // Servomoteur:
  //   Si le servo s'est bien positionné:
	//char servo = SERVO_update();
  //if (servo) print_MASTER("AS V\r\n>");

  // Pointeur lumineux:
  POINTEUR_update();
}

/**
 * Fonction à exécuter toutes les 100 ms pour mettre à jour tous les
 * périphériques
 */
void RTOS_100ms() {
  // Prise de vue::
  PHOTO_update();
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
  UART0_init();

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
  PHOTO_init();
  
  /**
   * Crossbar:
   */
  
  // UART0.TX: sortie en Push-Pull
  P0MDOUT |= 1;  // P0.0
  
  // SPI:
  P0MDOUT |= (1 << 3);   // Push-pull : MISO(P0.3)
  P0MDOUT &= ~((1 << 4) + (1 << 5));  // Input : MOSI(P0.4),  NSS(P0.5)
  // + SPI_MOSI = 1 dans SPI_SLAVE.c

  // Pointeur lumineux: sortie en Push-Pull
  P0MDOUT |= 1 << 6;  // CEX0(P0.?) 
}

/**
 * Arrêt d'urgence:
 *  Arrêt complet de la base roulante, les opérations en cours sont
 *  interrompues, les données en cours d’acquisition sont perdues
 */
void SLAVE_exit() { POINTEUR_stop(); }