#ifndef UART0_H
#define UART0_H

/**
 * Initialisation de l'UART0
 * Registres modifiés: SCON0 (SM00, SM01, REN0, RI0), IP (PS/PS0), IE (ES0)
 */
void UART0_init();

/**
 * Configuration du Timer 2 comme horloge de l'UART0
 * Registres modifiés: T2CON (TCLK0, RCLK0, TR2), RCAP2H, RCAP2L
 */
void TIMER2_config();

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
 * Définit la couleur des prochains caractères envoyés par l'UART0
 * @param {char} color : indice de la couleur 
 */
void UART0_setColor(char color);


/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART0
 * @param {char*} buffer : ligne reçu par l'UART0
 */
void UART0_receive_handle_buffer(char*);

#endif  // UART0_H