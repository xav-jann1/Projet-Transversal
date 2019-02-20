# Cartes

Programmes utilisés sur chaque carte du projet :
- [Master](./Master) : 8051
  - **Communications** : [UART](../Communication/UART), [SPI](../Communication/SPI)
  - **Actionneurs** : [Serializer](../Actionneurs/Serializer), [Servomoteur](../Actionneurs/Servomoteur)
  - **Capteurs** : [Courant](../Capteurs/Courant), Télémètres [Infrarouge](../Capteurs/Infrarouge) et [Ultrason](../Capteurs/Ultrason)

- [Slave](./Slave) : 8051
  - **Communications** : [SPI](../Communication/SPI), [Radio](../Communication/Radio)
  - **Actionneurs** : [Serializer](../Actionneurs/Serializer), [Servomoteur](../Actionneurs/Servomoteur), [Pointeur lumineux](../Actionneurs/Pointeur_lumineux)
  - **Capteurs** : [Caméra](../Capteurs/Caméra)

- [STM32](./STM32) :
  - **Communications** : [???](../Communication/???), [Bluetooth](../Communication/Bluetooth), [SD](../Communication/SD)
  - **Actionneurs** : [Haut-parleur](../Actionneurs/Haut-parleur)
  - **Capteurs** : [Microphone](../Capteurs/Microphone)

- [FPGA](./FPGA) :
  - **Communication** : [Radio](../Communication/Radio)
  - **Capteurs** : [Cible](../Capteurs/Cible)