# PCs

Liste de tous les PCs utilisés pour ce projet :
- [Centrale de commande](./Centrale_de_commande) : accès à l'IHM, et commmunique au serveur de **Calculs**
- [Calculs](./Calculs) : calcul la position des servos pour viser la cible, le chemin que doit parcourir le robot pour éviter les obstacles


## Utilisation

Pour utiliser les applications développées pour les PCs, il faut d'abord avoir installé [`Node.js`](https://nodejs.org/)
Ensuite, exécuter la commande `$ npm install` dans le dossier de l'application à tester, pour installer toutes les bibliothèques utilisées.
Enfin, exécuter `$ node .` dans le dossier de l'application pour lancer le programme.

Si **Centrale de Commande**: se connecter sur la page `http://localhost:3000` pour accéder à l'IHM.

Pour que la **Centrale de commande** fonctionne correctement, il est conseillé de lancer en même temps le **PC de Calculs**.