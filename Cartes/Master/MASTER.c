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

/**
 * Variables de la carte Master:
 */

// Epreuve en cours:
bit MASTER_isEpreuveInProgress_flag = 0;
unsigned char MASTER_epreuve = 1;

/**
 * Fonction principale 
 */
void main(void) {
  // Initialisation de la carte et des périphériques:
  MASTER_init();

  // Messages de démarrage:
  print_PC("\n\rMASTER init completed\n\r");
  TIME_wait(2000);
  print_PC("Waiting for instructions...\n\r");
  TIME_wait(1000);
  print_PC(">");
  UART0_resetColor();

  // Boucle principale:
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

}

/**
 * Démarrer une épreuve
 * @param {unsigned char} e : numéro de l'épreuve
 * @return {bit} 0: ok, 1: erreur -> une épreuve a déjà commencée, 
 *                                   ou numéro incorrect
 */
bit MASTER_startEpreuve(unsigned char e) {
  // Vérification:
  if (MASTER_isEpreuveInProgress_flag || e == 0 || e > 8) return 1;

  // Début de l'épreuve:
  MASTER_epreuve = e;
  MASTER_isEpreuveInProgress_flag = 1;

  return 0;
}

/**
 * Indique si une épreuve est en cours
 * @return {bit} 0: non, 1: épreuve en cours
 */
bit MASTER_isEpreuveInProgress() { return MASTER_isEpreuveInProgress_flag; }

/**
 * Terminer une épreuve
 * @return {bit} 0: ok, 1: erreur -> aucune épreuve en cours
 */
bit MASTER_endEpreuve() {
  // Vérification:
  if (!MASTER_isEpreuveInProgress_flag) return 1;

  MASTER_isEpreuveInProgress_flag = 0;
  return 0;
}

/**
 * Arrêt d'urgence:
 *  Arrêt complet de la base roulante, les opérations en cours sont
 *  interrompues, les données en cours d’acquisition sont perdues
 */
void MASTER_exit() {
  MASTER_isEpreuveInProgress_flag = 0;
  // BASE_stop();
}