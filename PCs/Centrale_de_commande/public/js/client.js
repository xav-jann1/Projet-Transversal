let socket;
setup();

function setup() {
  console.log('Initialisation ...');
  socket = io.connect('http://localhost:3000');
  socket.on('IHM', manageData);
}

function manageData(data) {
  console.log(data);
}

// Envoie une commande
function sendCommand(command) {
  console.log(`Button Pressed: ${command}`);
  socket.emit('IHM', command);
}

/*////////////////////////////////////////////////////////////////////
Carte 1 Moteur + servomoteur + position
*/////////////////////////////////////////////////////////////////////


// Vitesse moteur:
function motorPower(value) { sendCommand(`TV ${value}`); }

// Commande servomoteur:
function servoCommand() {
  var val1 = document.getElementById("angle1").value;
  var val2;
  if (document.getElementById("H").checked == true) val2 = "H";
  else val2 = "V";

  command = `CS ${val2} A:${val1}`;
  sendCommand(command)
}


// Commande position:
function positionCommand(id) { sendCommand(`${id}`); }


/*////////////////////////////////////////////////////////////////////
Carte 2 Mesure de distance
*/////////////////////////////////////////////////////////////////////

// Mesure courant: 
function courantMeasure(id) { sendCommand(`${id}`); }

// Mesure énergie:
function energieMeasure(id) { sendCommand(`${id}`); }


/*////////////////////////////////////////////////////////////////////
Carte Déplacements 1
*/////////////////////////////////////////////////////////////////////

// Commande au clic bouton:
function buttonPressed(id) { sendCommand(`${id}`); }

// Avancer:
function moveFoward(val1) {
  var val2 = document.getElementById("vitesse").value;
  let command = `A ${val2}`;
  sendCommand(command);
}

// Reculer:
function moveBackward(val1) {
  var val2 = document.getElementById("vitesse").value;
  let command = `B ${val2}`;
  sendCommand(command);
}

// Rotation de la base roulante avec choix de tout les paramètres:
function rotationParam() {
  var val1 = document.getElementById("angle2").value;
  var val2;
  if (document.getElementById("rot_droit").checked == true) val2 = "D";
  else val2 = "G";

  let command = `RA ${val2}:${val1}`;
  sendCommand(command);
}


/*////////////////////////////////////////////////////////////////////
Carte Déplacements 2 avec coordonnées
*/////////////////////////////////////////////////////////////////////

// Déplacement de la base roulante avec coordonnée et angle
function moveCoords() {
  var val1 = document.getElementById("valeur_x").value;
  var val2 = document.getElementById("valeur_y").value;
  var val3 = document.getElementById("valeur_angle").value;

  let command = `G X:${Math.floor(val1/10)} Y:${Math.floor(val2/10)} A:${val3}`;
  sendCommand(command);
}



/*////////////////////////////////////////////////////////////////////
Carte Pointeur
*/////////////////////////////////////////////////////////////////////

// Commande du choix de tout les paramètres
function pointeurParam() {
  var val1 = document.getElementById("Ia").value;
  var val2 = document.getElementById("D1").value;
  var val3 = document.getElementById("E1").value;
  var val4 = document.getElementById("N").value;

  let command = `L I:${val1} D:${val2} E:${val3} N:${val4}`;
  sendCommand(command);
}

/*////////////////////////////////////////////////////////////////////
Carte signaux sonores
*/////////////////////////////////////////////////////////////////////

// Configuration default de la génération de signaux sonore
function signalDefault() {
  let command = 'SD F:6 P:25 W:50 B:3';
  sendCommand(command);
}


// Temps d'acquisition du signal sonore
function acquisitionTime(value) {
  let command = `ASS ${value}`;
  sendCommand(command);
}

// Generation de signaux sonores avec tout les paramètres
function generationSon() {
  var val1 = document.getElementById("code_frequence").value;
  var val2 = document.getElementById("duree_son").value;
  var val3 = document.getElementById("duree_silence").value;
  var val4 = document.getElementById("nombres_bips").value;

  let command = `SD F:${val1} P:${val2} W:${val3} B:${val4}`;
  sendCommand(command);
}


/*////////////////////////////////////////////////////////////////////
Acquisition visuelle
*/////////////////////////////////////////////////////////////////////

//mode par défault de l'aquisition visuelle
function defaultVision() {
  let command = "PPH O E:1 N:1";
  sendCommand(command);
}


//choix des paramètres pour l'acquisition visuelle
function visuelParam() {
  var val1;
  var val2 = document.getElementById("duree_allumage").value;
  var val3 = document.getElementById("nbr_prise").value;
  if (document.getElementById("O1").checked == true) val1 = "O";
  if (document.getElementById("L1").checked == true) val1 = 'L';
  else val1 = "S";

  let command = `PPH ${val1} E:${val2} N:${val3}`
  sendCommand(command);
}

/*////////////////////////////////////////////////////////////////////
Carte canva
*/////////////////////////////////////////////////////////////////////

