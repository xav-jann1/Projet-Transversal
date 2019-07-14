#ifndef UART0_H
#define UART0_H

// Couleurs du texte:
enum color { WHITE, RED, GREEN, YELLOW, BLUE, PINK, CYAN, GREY };

/**
 * Initialisation de l'UART0
 * Registres modifiés: SCON0 (SM00, SM01, REN0, RI0), IP (PS/PS0), IE (ES0)
 */
void UART0_init();

/**
 * Configuration du Timer 1 comme horloge de l'UART0
 * Registres modifiés: TMOD (T1M1,T1M0), T2CON (TCLK0, RCLK0),
 *                     CKCON (T1M), TCON (TR1), TH1
 */
void TIMER1_config();

/**
 * Fonction de mise à jour de l'UART0
 */
void UART0_update();

/**
 * Envoie un caractère par l'UART0
 * @param {char} c : caractère à envoyer
 */
void UART0_sendChar(char);

/**
 * Envoie une chaîne de caractères par l'UART0
 * @param {char*} string : chaîne de caractères à envoyer
 */
void UART0_send(char*);

/**
 * Envoie 3 chaînes de caractères à la suite par l'UART0
 * @param {char*} string{1-3} : chaîne de caractères à envoyer
 */
void UART0_send3(char* string1, char* string2, char* string3);

/**
 * Définit la couleur des prochains caractères envoyés par l'UART0
 * @param {char} color : indice de la couleur
 */
void UART0_setColor(char color);

/**
 * Définit la couleur par défaut des caractères envoyés par l'UART0
 */
void UART0_resetColor();

/**
 * ARCHIVES
 */

/**
 * Configuration du Timer 2 comme horloge de l'UART0
 * Registres modifiés: T2CON (TCLK0, RCLK0, TR2), RCAP2H, RCAP2L
 */
// void TIMER2_config();

#endif  // UART0_H