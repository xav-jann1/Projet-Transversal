# Cartes Master et Slave

Rassemblement de tous les programmes développés pour les cartes 8051.

Toutes les fonctionnalités utilisées pour ces cartes ont été réparties en fonction de leur utilisation :
- [Actionneurs](./Actionneurs)
- [Capteurs](./Capteurs)
- [Communication](./Communication)

## [Cartes](./Cartes)

- [Master](./Cartes/Master) : 8051
  - **Communications** : [UART](./Communication/UART), [SPI](./Communication/SPI)
  - **Actionneurs** : [Serializer](./Actionneurs/Serializer), [Servomoteur](./Actionneurs/Servomoteur)
  - **Capteurs** : [Courant](./Capteurs/Courant), Télémètre [Ultrason](./Capteurs/Ultrason)

- [Slave](./Cartes/Slave) : 8051
  - **Communications** : [SPI](./Communication/SPI)
  - **Actionneurs** : [Serializer](./Actionneurs/Serializer), [Servomoteur](./Actionneurs/Servomoteur), [Pointeur lumineux](./Actionneurs/Pointeur_lumineux)
  - **Capteurs** : [Photo](./Capteurs/Photo)

---

## Actionneurs
- [Pointeur lumineux](./Actionneurs/Pointeur_lumineux) : contrôle du pointeur lumineux
- [Serializer](./Actionneurs/Serializer) : déplacement de la base
- [Servomoteur](./Actionneurs/Servomoteur) : contrôle de l'orientation d'un servomoteur

## Capteurs
- [Courant](./Capteurs/Courant) : mesure du courant par une résistance de Shunt
- [Microphone](./Capteurs/Microphone) : acquisition d'un signal sonore
- [Photo](./Capteurs/Photo) : utilisation de la caméra pour les prises de vue
- [Ultrason](./Capteurs/Ultrason) : mesure d'une distance par un télémètre ultrason

## Communication
- [Parseur de messages](./Communication/Parseur_messages) : extrait le commande d'un message reçu par la liaison *UART* ou *SPI*
- [SPI](./Communication/SPI) : communication *SPI* entre la **Carte Master** et la **Carte Slave**
- [UART](./Communication/UART) : communication *UART* entre la **Carte Master** et la **Centrale de Commande**

