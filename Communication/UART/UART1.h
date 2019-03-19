#ifndef UART1_H
#define UART1_H

/**
 * Initialisation de l'UART1
 * Registres modifiés: SCON0 (SM00, SM01, REN0, RI0), IP (PS/PS0), IE (ES0)
 */
void UART1_init();

/**
 * Configuration du Timer 1 comme horloge de l'UART1
 * Registres modifiés: TMOD (T1M1,T1M0), T4CON (TCLK1, RCLK1), 
 *                     CKCON (T1M), TCON(TR1), TH1
 */
void UART1_TIMER1_config();

/**
 * Fonction de mise à jour de l'UART1
 */
void UART1_update();

/**
 * Envoie un caractère par l'UART1
 * @param {char} c : caractère à envoyer
 */
void UART1_sendChar(char);

/**
 * Envoie une chaîne de caractères par l'UART1
 * @param {char*} string : chaîne de caractères à envoyer
 */
void UART1_send(char*);

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART1
 * @param {char*} buffer : ligne reçu par l'UART1
 */
void UART1_receive_handle_buffer(char*);

#endif  // UART1_H

/**
 * Archives:
 */

/**
 * Configuration du Timer 4 comme horloge de l'UART1
 * Registres modifiés: T2CON (TCLK0, RCLK0, TR2), RCAP2H, RCAP2L
 */
// void UART1_TIMER4_config();