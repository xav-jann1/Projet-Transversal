# Serializer

Déplacement de la base du robot par la communication avec une carte **Serializer** qui contrôle les **moteurs**.

## Communication

La carte **Serializer** utilise une interface série **UART** :

### UART :
  - **Baud Rate** : 19200
  - **Data bits** : 8
  - **Stop bit** : 1
  - **No parity**
  - **No Flow Control**
  
### Protocole :
- Toutes les **commandes** doivent se **terminer**, pour être **interprétées**, par le **caractère** `<CR>` (`0x0D` ou `\r`) 
- Toutes les **réponses se finissent** par `<CR><LF>` + `>`  (`\r\n>`)
- Si la **commande a été comprise** et **correctement exécutée** : `ACK<CR><LF>` + `>` (`ACK\r\n>`)
- Si la **commande n'a pas été comprise** : `NACK<CR><LF>` + `>`  (`NACK\r\n>`)

## Informations utiles

- Ne pas oublier d'ignorer au démarrage, la séquence d'initialisation de la carte **Serializer**
- Au démarrage, toujours définir le `VPID` et le `DPID` (cf. page 35 de *Serializer - User Guide*)

## Caractéristiques de la base

### Motoréducteurs :
  - Rapport de réduction : `1/52`
  - Vitesse de rotation maximale en sortie de réducteur : `160 tours/min`

### Codeurs :
  - 2 voies en `quadrature`
  - `12` impulsions par tour moteur
  - `624` impulsions par tour axe sortie motoréducteur
  
---
## Commandes

### Configuration :

Commande|Action
-|-
`fw`                    | Retourne la version du *Firmware*
`reset`                 | **Initialise** et **redémarre** la carte
`restore`               | **Réinitialise** la carte avec les paramètres par défaut d'usine
||
`cfg enc [encoderType]`  |  Configure l'encodeur (*simple* ou *quadrature*)
`cfg baud [baudRate]`   |  Configure le **Baud Rate** de la carte
`cfg units [unit type]` |  **Unité** utilisée pour la lecture de capteurs (métrique ou impérial)
||
`vpid [prop:integ:deriv:loop]`   |  Récupère ou définit les paramètres du **PIDL** pour le PID
`dpid [prop:integ:deriv:accel]`  |  Récupère ou définit les paramètres du **PIDA** pour le PID
`rpid s\|t`                      |  Redéfinit les paramètres *par défaut* du **PID** (pour des robots connus : *Stinger* ou *Traxster*)


### Utilisation :
Commande|Action
-|-
`clrenc encoderId [encoderId]`            |  **Initialise** les compteurs d'encodeur(s)
`getenc encoderId [encoderId]`            |  Retourne la valeur des **compteurs** des encodeurs
||
`digo id:distance:vel [id:distance:vel]`  |  Déplace la base à une **distance** avec une **vitesse** données (id=1,2)
`mogo motorId:vel [motorId:vel]`          |  Contrôle la **vitesse** des moteurs (1 ou 2) avec **PID**
`pwm [r:rate] id:pwm [id:pwm]`            |  Contrôle la **vitesse** des moteurs par **PWM** (sans PID)
||
`vel`                                     |  Retourne la **vitesse** des roues *gauche* et *droite*
`pids`                                    |  Renvoie si le **PID** est en fonctionnement
||
`stop`                                    | **Arrête** tous les moteurs


---
## Autres Commandes du Serializer

Commande|Action
-|-
`blink ledId:rate [ledId:rate]`   |  Fait clignoter la LED sur la carte
||
`setio pinId:value [pinId:value]` |  Définit l'état de GPIOs sur la carte
`getio pinId [pinId]`             |  Récupère l'état de GPIOs
||
`i2cp currAddr newAddr`           |  Change l'adresse d'un périphérique I2C
`i2c r\|w addr [data]`             |  Utilisation de la communication I2C
||
`step dir:speed:steps`            |  Contrôle d'un moteur pas à pas
`sweep speed:steps`               |  Contrôle des moteurs pas à pas pour faire balayer le robot
||
`sensor id [idN]`                 |  Renvoie la valeur d'un capteur
`servo id:pos [id:pos]`           |  Contrôle de servomoteur(s)
||
`cmps03 [i2c addr]`               |  Retourne la valeur de la boussole
`maxez1 triggerPin outputPin`     |  Utilisation du sonar Maxbotix MaxSonar-EZ1
`sp03 [i2cAddr]`                  |  Utilisation du Synthétiseur de voix
`srf04 triggerPin outputPin`      |  Utilisation d'un sonar SRF04 branché sur les GPIOs
`srf05 pinId` ou `pping pinId`    |  Utilisation d'un sonar SRF05/Ping branché sur les GPIOs
`srf08 [i2cAddr]` ou `srf08 [i2cAddr]` |  Utilisation d'un sonar SRF08/SRF10 branché sur les GPIOs
`tpa81 [i2cAddr]`                 |  Utilisation du capteur de température Devantech TPA81
`line[7] addr [-a addr]`          |  Utilisation de suiveurs de lignes

