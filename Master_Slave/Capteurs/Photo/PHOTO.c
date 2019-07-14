
#include "c8051f020.h"
#include "PHOTO.h"

/**
 * Variables pour la prise de vue
 */

sbit PHOTO_out = P1 ^ 5;

bit PHOTO_inProgress = 0;
int PHOTO_compteur_100ms = 0;
char PHOTO_mode = 'O';
unsigned char PHOTO_E = 1, PHOTO_compteur = 0, PHOTO_N = 1;

/**
 * Configuration de la sortie connecté à la Raspberry:
 * Registre modifiés: P1MDOUT
 */
void PHOTO_init() { P1MDOUT |= 1 << 5; }

/**
 * Fonction déclenchée toutes les 100 ms pour les prises de vues
 */
void PHOTO_update() {
  // Si prise de vu en cours:
  if (PHOTO_inProgress) {
    if (PHOTO_mode == 'O') {
      PHOTO();
      PHOTO_stop();
    }

    else {
      PHOTO_compteur_100ms++;

      // Pour prendre une photo toutes les E * 100ms:
      if (PHOTO_compteur_100ms == PHOTO_E) {
        PHOTO_compteur_100ms = 0;
				
				// Prend une photo:
        PHOTO();
        
				
        if (PHOTO_mode == 'S') {
          PHOTO_compteur++;
					// Si toutes les photos ont été prises:
          if (PHOTO_compteur == PHOTO_N) PHOTO_stop();
        }
      }
    }
  }
}

// Fin d'une prise de vues
void PHOTO_stop() { PHOTO_inProgress = 0; }

/**
 * Démarrage de la prise de vue avec la caméra
 * @param {char} mode : mode de la capture ('O', 'C', ou 'S')
 * @param {unsigned char} E : durée entre 2 clichés (0 - 99)
 * @param {unsigned char} N : nombre de clichés (1 - 255)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit PHOTO_start(char mode, unsigned char E, unsigned char N) {
  // Vérification:
  if (mode != 'O' && mode != 'C' && mode != 'S') return 1;
  if (E < 1 || E > 99) return 1;
  if (N == 0) return 1;

  // Enregistrement:
  PHOTO_mode = mode;
  PHOTO_E = E;
  PHOTO_N = N;

  // Début de la capture:
  PHOTO_inProgress = 1;
  PHOTO_compteur = 0;

  return 0;
}

void PHOTO() { PHOTO_out = PHOTO_out ^ 1; }