#include "ULTRA.h"
#include "c8051F020.h"

#define SYSCLK 22118400

/**
* Organisation des pins :
 *   Capteur avant:
 *     - Trig : P3.3
 *     - Echo : P3.6
 *   Capteur arrière:
 *     - Trig : P3.4
 *     - Echo : P3.7
 *
 * Pins du port : (!!! : 1 sur le port 3 indique port P3.0)
 *     *7 *6
 *     *5 *4
 *     *3 *2
 *     *1 *0     
 */

/**
 * Variables du capteur Ultrason:
 */

// Pins des capteurs Ultrasons avant et arrière:
sbit ULTRA_avant_trig = P3 ^ 3;
sbit ULTRA_arriere_trig = P3 ^ 4;

// Distance mesurée:
unsigned int ULTRA_mesure_cm;

// Flag de mesure:
bit ULTRA_mesure_flag = 0;
char ULTRA_double_mesures_step = '\0';  // 'f': "forward", 'b': "backward"

// Aide pour la mesure: délai entre deux coups de SYSCLK/12
float TIMER_ticks_in_us = 1.0f / (SYSCLK / 12.0f * 0.000001);

/**
 * Initialisation des capteurs Ultrason
 * Registres modifiés: P1MDOUT, P3MDOUT, P3IF(IE6, IE7)
 */
void ULTRA_init() {
  // Pins du capteur avant:
  P3MDOUT |= 1 << 3;  // Sortie en Push-Pull
  ULTRA_avant_trig = 0;

  // Activation interruption External Interrupt 6:
  EIP2 |= 1 << 4;     // Priorité élevée: PX6 = 1
  P3IF &= ~(1 << 6);  // RAZ Flag: IE6 = 0
  EIE2 |= 1 << 4;     // Enable: EX6 = 1

  // Pins du capteur arrière:
  P3MDOUT |= 1 << 4;  // Sortie en Push-Pull
  ULTRA_arriere_trig = 0;

  // Activation interruption External Interrupt 7:
  EIP2 |= 1 << 5;     // Priorité élevée: PX7 = 1
  P3IF &= ~(1 << 7);  // RAZ Flag: IE7 = 0
  EIE2 |= 1 << 5;     // Enable: EX7 = 1

  // Configuration du Timer 4:
  TIMER4_config();
}

/**
 * Configuration du Timer 4 pour mesurer une impulsion
 * Registres modifiés: T4CON, CKCON (T4M)
 */
void TIMER4_config() {
  // Reset du Timer 4:
  T4CON = 0;

  // Utilise SYSCLK / 12:
  CKCON &= ~(1 << 6);  // T4M = 0

  // Active interruption:
  T4CON &= ~(1 << 7);  // RAZ Flag: TF4 = 0
  EIE2 |= 1 << 2;      // Enable: ET4 = 1
}

/**
 * Fonction qui doit s'exécuter toutes les ms,
 * pour prévenir lorsque le capteur ultrason à réalisé une mesure
 * @return {bit} 0: rien, 1: mesure réalisée
 */
bit ULTRA_update() {
  // Si une mesure est réalisée:
  if (ULTRA_mesure_flag == 1) {
    ULTRA_mesure_flag = 0;
    
    // Si pendant 'double mesures':
    if (ULTRA_double_mesures_step == 'f') {
      // Lance la mesure arrière:
      ULTRA_mesure_arriere();
      ULTRA_double_mesures_step = 'b';  
    }
    
    // Indique qu'une mesure a été réalisée:
    return 1;
  }

  return 0;
}

/**
 * Renvoie la valeur d'une mesure réalisée
 * @return {int} distance mesurée
 */
int ULTRA_getMesure() { return ULTRA_mesure_cm; }

/**
 * Delay de ~10us pour le trigger du télémètre
 */
void ULTRA_delay_10us() {
  int i;
  for (i = 0; i < 18; i++)
    ;
}

/**
 * Préparation pour une mesure avec un télémètre ultrason
 * Registres modifiés: T4CON(TR4), TL4, TH4 P3IF(IE6CF)
 */
void ULTRA_mesure_init() {
  // Désactive le Timer 4:
  T4CON &= ~(1 << 2);  // TR4 = 0

  // Initialise le compteur du Timer 4:
  TL4 = 0;
  TH4 = 0;

  // Réinitialise le flag indiquant la fin de la mesure:
  ULTRA_mesure_flag = 0;
}

/**
 * Déclenche une mesure avec le télémètre avant
 */
void ULTRA_mesure_avant() {
  // Préparation:
  ULTRA_mesure_init();

  // Détection sur front montant External Interrupt 6:
  P3IF |= 1 << 2;  // IE6CF = 1

  // Déclenche le trigger du capteur:
  EA = 0;
  ULTRA_avant_trig = 1;
  ULTRA_delay_10us();
  ULTRA_avant_trig = 0;
  EA = 1;
}

/**
 * Déclenche une mesure avec le télémètre arrière
 */
void ULTRA_mesure_arriere() {
  // Préparation:
  ULTRA_mesure_init();

  // Détection sur front montant External Interrupt 7:
  P3IF |= 1 << 3;  // IE7CF = 1

  // Déclenche le trigger du capteur:
  EA = 0;
  ULTRA_arriere_trig = 1;
  ULTRA_delay_10us();
  ULTRA_arriere_trig = 0;
  EA = 1;
}

/**
 * Réalise une double mesures: 
 *  capteur avant, puis capteur arrière (si le capteur avant n'a rien détecté)
 */
void ULTRA_double_mesures() {
  // Commence par le capteur avant:
  ULTRA_mesure_avant();
  ULTRA_double_mesures_step = 'f';
}

/**
 * Fonction d'interruption de External Interrupt 6 sur le pin P3.6
 * Mesure de l'impulsion de la sortie Echo du télémètre avant
 * et conversion en distance (en cm)
 * Registres modifiés: T4CON(TR4), P3IF(IE6CF, IE6)
 */
void ULTRA_interrupt_mesure_avant() interrupt 18 {
  // Si détection d'un front montant (début de l'impulsion):
  if ((P3IF >> 2) & 1 == 1) {
    // Active le Timer 4:
    T4CON |= 1 << 2;  // TR4 = 1

    // Détection sur front descendant External Interrupt 6:
    P3IF &= ~(1 << 2);  // IE6CF = 0
  }

  // Sinon, détection d'un front descendant (fin de l'impulsion):
  else {
    int pulse_width;

    // Désactive le Timer 4:
    T4CON &= ~(1 << 2);  // TR4 = 0

    // Durée de l'impulsion mesurée en us:
    pulse_width = ((TH4 << 8) + TL4) * TIMER_ticks_in_us;

    // Calcul de la distance: (cf. datasheet)
    ULTRA_mesure_cm = pulse_width / 58;

    // Détection sur front montant External Interrupt 6:
    P3IF |= 1 << 2;  // IE6CF = 1

    // Informe qu'une mesure a été réalisée:
    ULTRA_mesure_flag = 1;
  }

  // RAZ flag External Interrupt 6:
  P3IF &= ~(1 << 6);  // IE6 = 0
}

/**
 * Fonction d'interruption de External Interrupt 7 sur le pin P3.7
 * Mesure de l'impulsion de la sortie Echo du télémètre arrière
 * et conversion en distance (en cm)
 * Registres modifiés: T4CON(TR4), P3IF(IE7CF, IE7)
 */
void ULTRA_interrupt_mesure_arriere() interrupt 19 {
  // Si détection d'un front montant (début de l'impulsion):
  if ((P3IF >> 3) & 1 == 1) {
    // Active le Timer 4:
    T4CON |= 1 << 2;  // TR4 = 1

    // Détection sur front descendant External Interrupt 7:
    P3IF &= ~(1 << 3);  // IE7CF = 0
  }

  // Sinon, détection d'un front descendant (fin de l'impulsion):
  else {
    int pulse_width;

    // Désactive le Timer 4:
    T4CON &= ~(1 << 2);  // TR4 = 0

    // Durée de l'impulsion mesurée en us:
    pulse_width = ((TH4 << 8) + TL4) * TIMER_ticks_in_us;

    // Calcul de la distance: (cf. datasheet)
    ULTRA_mesure_cm = pulse_width / 58;

    // Si pas d'obstacle:
    if (ULTRA_mesure_cm > 200) ULTRA_mesure_cm = 0;

    // Détection sur front montant External Interrupt 7:
    P3IF |= 1 << 3;  // IE7CF = 1

    // Informe qu'une mesure a été réalisée:
    ULTRA_mesure_flag = 1;
  }

  // RAZ flag External Interrupt 7:
  P3IF &= ~(1 << 7);  // IE7 = 0
}

/**
 * Fonction d'interruption du Timer 4 pour détecter quand il n'y pas d'obstacle
 */
void ULTRA_interrupt_Timer4() interrupt 16 {
  // Désactive le Timer 4:
  T4CON &= ~(1 << 2);  // TR4 = 0

  // RAZ Flag:
  T4CON &= ~(1 << 7);  // TF4 = 0

  // Distance indéfinie:
  ULTRA_mesure_cm = 0;

  // Informe qu'une mesure a été réalisée:
  // if(!ULTRA_mesure_flag) ULTRA_mesure_flag = 1;
}