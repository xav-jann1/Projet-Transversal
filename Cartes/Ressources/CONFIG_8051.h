#ifndef CONFIG_8051_H
#define CONFIG_8051_H

// Désactivation du Watchdog
void CONFIG_watchdog();

// Utilisation de l'oscillateur externe:
void CONFIG_externClock();

// Autorise les interruptions:
void CONFIG_activeInterrupts();

// Séquence d'initialisation de la carte:
void CONFIG_init();

#endif  // CONFIG_8051_H