let terrain, chemin;

// Chargement des fichiers:
function preload() {
  //terrain = loadJSON('./Fichiers JSON Exemples/Description Environnement v10-05-17.json');
  //chemin = loadJSON('./Fichiers JSON Exemples/Evolution Base Roulante v10-05-17.json');
}


function setup() {
  console.log("Démarrage")
  let canvas = createCanvas(700, 500);
  background(51);

  // Move the canvas so it’s inside our <div id="sketch-holder">.
  canvas.parent('sketch-holder');

  canvas.dragOver(() => {
    background(20);
  })
  canvas.dragLeave(() => {
    background(51);
    showText();
  })

  let filesLoaded = [];
  canvas.drop((file) => {
    console.log(file.name);
    if (file.name.includes('Environnement')) {
      console.log('Environnement');
      terrain = JSON.parse(atob(file.data.split('base64,')[1]));
      if (!filesLoaded.includes('Environnement')) filesLoaded.push('Environnement');
    }
    if (file.name.includes('Base')) {
      console.log('Base');
      chemin = JSON.parse(atob(file.data.split('base64,')[1]));
      if (!filesLoaded.includes('Base')) filesLoaded.push('Base');
    }

    showText();

    console.log(filesLoaded);
    console.log(filesLoaded.includes('Environnement'), filesLoaded.includes('Base'))

    if (filesLoaded.includes('Environnement') && filesLoaded.includes('Base')) {
      drawTerrain();
      socket.emit('trajectoire', { terrain, chemin });

      // Pour la position du pointeur:
      let data = {
        Xc: terrain.cible.centre.x,
        Yc: terrain.cible.centre.y,
        Zc: terrain.cible.hauteur,
        Xr: chemin["position-tir_cible"].coordonnees.x,
        Yr: chemin["position-tir_cible"].coordonnees.y
      }
      socket.emit('pointeur', data);
    }
  })

  socket.on('trajectoire', trajectoire => {

    //console.log(trajectoire);

    let x = chemin.depart.coordonnees.x;
    let y = chemin.depart.coordonnees.y;
    //console.log(x, y);

    push();

    // Inversion de l'axe Y:
    scale(1, -1);

    // Positionnement du terrain par rapport à l'affichage:
    translate(150, -400);

    stroke('#0091EA');
    strokeWeight(2);
    rect(-250, -250, 100, 100);
    for (let vec of trajectoire) {
      line(x, y, x + vec.x, y - vec.y);
      x += vec.x;
      y -= vec.y;
    }

    drawPoints();

    pop();

    // Modification des points pour correspondre aux déplacements:
    let trajectoire_corrige = trajectoire;
    if (chemin.depart.angle == 0) trajectoire_corrige = trajectoire.map(vec => ({ x: vec.y, y: vec.x }));
    else if (chemin.depart.angle == 90) trajectoire_corrige = trajectoire.map(vec => ({ x: vec.x, y: -vec.y }));

    console.log("Coordonnées pour déplacer la base: G X:_ Y:_ A:0");
    console.log(trajectoire_corrige)
  });

  socket.on('pointeur', pos => {
    console.log('Position du pointeur:', pos);
  });

  showText();


  function showText() {
    // Texte Glisser-Déposer:
    fill(255); textSize(20);
    textAlign(CENTER, CENTER);
    text(`Glisser-Déposer les fichiers\npour les charger... (${filesLoaded.length}/2)`, width / 2, height / 2);
  }
}


function drawTerrain() {
  background(51);

  // Inversion de l'axe Y:
  scale(1, -1);

  // Positionnement du terrain par rapport à l'affichage:
  translate(150, -400);

  // Affichage de la grille:
  showGrid(25, 25);

  /** Terrain **/

  // Contour du terrain:
  fill(100, 80); stroke(255);
  drawShape(terrain.terrain_evolution);
  noFill();
  drawShape(terrain.contour_cible);

  // Origine du repère:
  fill(100); stroke(150);
  ellipse(0, 0, 10, 10);

  // Obstacles:
  fill(255);
  drawObstacles(terrain.obstacles);

  // Cible:
  let x = terrain.cible.centre.x;
  let y = terrain.cible.centre.y;
  noFill(); stroke(255);
  ellipse(x, y, 40, 40);


  /** Chemin **/

  // Unité des angles:
  if (chemin.unite_angulaire == 'degre') angleMode(DEGREES);

  // Affiche les points:
  drawPoints();

}

function draw() { }

// Affiche tous les points du chemin:
function drawPoints() {
  // Départ:
  drawPoint(chemin.depart, 'lime');

  // Arrivée:
  drawPoint(chemin.arrivee, 'orangered');

  // Etapes:
  for (let point of chemin.etapes) {
    drawPoint(point, '#FFF');
  }

  // Tir cible:
  drawPoint(chemin['position-tir_cible'], '#999');

}


// Affiche les obstacles du terrain:
function drawObstacles(obstacles) {
  // Pour chaque obstacle:
  for (let obstacle of obstacles) {

    // Données de l'obstacle:
    let type = obstacle.type;
    let coords = obstacle.coordonnees;

    // Si c'est un polygone:
    if (type == 'polygone') {
      drawShape(coords);
    }

    // Si c'est un cercle:
    else if (type == 'cercle') {

      // Paramètres du cercle:
      let x = 0, y = 0, rayon = 10;

      // Récupère les données du cercle:
      for (let coord of coords) {
        if ('centre' in coord) { x = coord.centre.x; y = coord.centre.y; }
        else if ('rayon' in coord) rayon = coord.rayon;
      }

      // Affiche le cercle:
      let diametre = 2 * rayon;
      ellipse(x, y, diametre, diametre);
    }

  }
}

// Affiche une figure constituée de points:
function drawShape(coords) {
  beginShape();  // Début de la figure

  // Ajoute les points à la figure:
  for (let p of coords) {
    let x = p.point.x;
    let y = p.point.y;
    vertex(x, y);
  }

  endShape(CLOSE);  // Fin de la figure
}

// Affiche un point à partir de coordonnées et d'un angle:
// Liste de couleurs : https://www.december.com/html/spec/colorsvg.html
function drawPoint(point, color = '#FFF') {
  // Récupère les données du point:
  let x = point.coordonnees.x;
  let y = point.coordonnees.y;
  let angle = point.angle;

  // S'il y a un angle:
  if (angle !== null) {
    // Ajoute un trait orienté selon l'angle:
    push();
    translate(x, y); rotate(angle);
    stroke(255);
    line(0, 0, 20, 0);
    pop();
  }

  // Affiche le point:
  fill(color); noStroke();
  ellipse(x, y, 10, 10);
}

// Affiche la grille avec des carrés de (w,h) en (longueur, largeur):
function showGrid(w, h) {
  stroke(70);

  // Abscisses:
  for (let x = -width; x < width; x += w) {
    line(x, -height, x, height);
  }

  // Ordonnées:
  for (let y = -height; y < height; y += h) {
    line(-width, y, width, y);
  }
}