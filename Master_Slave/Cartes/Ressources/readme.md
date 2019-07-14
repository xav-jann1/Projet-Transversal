# Ressources

## Objectif

- Rassembler des fonctions de configuration du 8051

## Détails

- `CONFIG_8051` : fonctions pour configurer facilement le 8051
  - Désactiver *watchdog* : `CONFIG_watchdog()`
  - Activer l'horloge externe : `CONFIG_externClock()`
  - ...

- `TIME_8051` : gestion du temps
  - `TIME_flag_ms`: Renvoie `1` si une milliseconde vient de passer
  - `TIME_flag_100ms`: Renvoie `1` si `100` millisecondes viennent de passer