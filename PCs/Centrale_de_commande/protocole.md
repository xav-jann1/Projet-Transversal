# Protocole de communication avec la carte Master

Description du protocole de communication utilisé entre la **Centrale de Commande** et la carte **Master** :

## Constitution d'un message

Chaque message transmis est constitué de deux éléments :
- **code de l'information**
- **1 ou plusieurs paramètres** : lorsque le paramètre est complexe le caractère `:` permet de séparer le mot clé avec sa valeur 

Pour correctement indiquer la fin d'un message, celui-ci doit se terminer par le caractère `<CR>` (`0x0D` ou `\r`)

## Accusé de réception

Pour fiabiliser les échanges, il est possible d'implémenter un accusé de réception pour les messages de "Commande" et d' "Information" :

**Accusé de réception d'un message "commande"** (émis par la carte Master) :
- Message correctement reçu : `<CR> <LF> >` soit `0x0D 0x0A 0x3E`
- Message incorrectement reçu : `<CR> <LF> #` soit `0x0D 0x0A 0x23`

**Accusé de réception d'un message "information"** (émis par la Centrale de Commande) :
- Message correctement reçu : `<CR> <LF> !` soit `0x0D 0x0A 0x21`
- Message incorrectement reçu : `<CR> <LF> ?` soit `0x0D 0x0A 0x3F`


## Commandes envoyées à la carte Master

Liste de toutes les commandes qui permet de communiquer entre la **Centrale de Commande** et la **carte Master**

**Fonctionnement :**

Commande | Détail
-|-
`D [type_épreuve]` | Début de l'épreuve
`E`                | Fin de l'épreuve
`Q`                | Arrêt "Urgence"


**Déplacement :**

Commande | Détail
-|-
`TV vitesse`       | Réglage de la vitesse de déplacement
`A [vitesse]`      | Avancer
`B [vitesse]`      | Reculer
`S`                | Fin des commandes de déplacement « A » et « B » de la base roulante
`RD`               | Rotation à droite de 90°
`RG`               | Rotation à gauche de 90°
`RC [D/G]`         | Rotation complète de la base roulante de 180°
`RA [sens:valeur]` | Rotation de la base roulante d’un angle donné (par défaut, droite - 90°)
`G X:valeur_x Y:valeur_y A:angle`   | Déplacement de la base roulante par coordonnées
||
`IPO X:valeur_x Y:valeur_y A:angle` | Initialisation de la Position de la base roulante
`POS`                               | Position de la base roulante


**Détection et Mesures :**

Commande | Détail
-|-
`MOU [D]`                          | Détection d’obstacle unique
`MOB [D] [A:résolution_angulaire]` | Détection des obstacles par balayage
`MOS [D] [A:résolution_angulaire]` | Détection de l’obstacle le plus proche par balayage
||
`MI`| Mesure courant
`ME`| Mesure énergie


**Signaux sonores :**

Commande | Détail
-|-
`ASS [durée]`                                                           | Acquisition de signaux sonores
`SD [F:code_frequence] [P:durée_son] [W:durée_silence] [B:nombre_bips]` | Génération de signaux sonores


**Pointeur lumineux :**

Commande | Détail
-|-
`L [I:Intensité] [D:Durée] [E:Durée] [N:Nombre]` | Allumage du pointeur lumineux
`LS`                                             | Fin de l’allumage du pointeur lumineux
`CS [H/V] [A:angle]`                             | Pilotage de servomoteur


**Prise de vue :**

Commande | Détail
-|-
`PPH [O/C/S] [E:Durée] [N:Nombre]` | Prise de photographie
`SPH`                              | Arrêt de la prise de photographies en continu

**Auxiliaires :**

Commande | Détail
-|-
`AUX []` | Commandes Auxiliaires

 
## Informations 
Commande | Détail
-|-
`I [chaine_de_caractères]` | Invite de Commandes
`B`                        | Arrivée au point spécifié
`AS [H/V]`                 | Servomoteur positionné
`KI courant_mesuré`        | Transmission mesure de courant
`KE énergie_consommée`     | Transmission énergie consommée
`KOB angle_1:distance_obstacle_1 … angle_n:distance_obstacle_n` | Transmission Informations de détection d’obstacles
`VPO X:valeur_x Y:valeur_y A:angle`  | Position de la base roulante
`IA [chaine_de_caractères]`          | Informations auxiliaires
