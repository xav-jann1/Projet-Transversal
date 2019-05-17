# Servomoteur

Contrôle de la position d'un servomoteur.

## Utilisation

- `SERVO_init()` : initialisation pour envoyer des pulsions au servomoteur
- `SERVO_pos(char pos)` : déplace le servomoteur à l'angle `pos` (entre `-90°` et `90°`)

## Fonctionnement

- Utilise le **Timer 3** pour générer une interruption de la durée de l'impulsion qui permet d'obtenir l'angle désiré.
- La sortie branchée au servomoteur passe à l'**état haut** que pendant **une interruption sur 10**.
