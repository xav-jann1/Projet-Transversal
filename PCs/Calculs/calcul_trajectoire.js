// Declaration des variables
let cols = 15, rows = 15;
let grid = new Array(cols);

let grid_temp = [];
let openSet = [];
let closedSet = [];

let start;  // Noeud de départ
let end;    // Noeud d'arrivée
let path = [];

let w, h;

exports.compute = (terrain, chemin) => {
  console.log("Start");
  //const fs = require('fs');
  //let terrain = JSON.parse(fs.readFileSync('./Description-Environnement-v10-05-17.json', 'utf8'));
  //let chemin = JSON.parse(fs.readFileSync('./Evolution-Base-Roulante-v10-05-17.json', 'utf8'));

  create_grid(terrain);

  let etapes = chemin.etapes;

  let from_x = chemin.depart.coordonnees.x;
  let from_y = chemin.depart.coordonnees.y;

  for (let i = 0; i < etapes.length; i++) {
    let to_x = chemin.etapes[i].coordonnees.x;
    let to_y = chemin.etapes[i].coordonnees.y;
    vec.concat(init(from_x, from_y, to_x, to_y));
    from_x = to_x;
    from_y = to_y;
  }

  let to_x = chemin["position-tir_cible"].coordonnees.x;
  let to_y = chemin["position-tir_cible"].coordonnees.y;
  vec.concat(init(from_x, from_y, to_x, to_y));

  from_x = to_x;
  from_y = to_y;
  to_x = chemin.arrivee.coordonnees.x;
  to_y = chemin.arrivee.coordonnees.y;
  vec.concat(init(from_x, from_y, to_x, to_y));

  const final = vec.map(x => x.mul(6.25).r());

  //console.log(final);

  return final;
}


// Fonction permettant d'enlever un élément d'un tableau
// @param: arr, element
// void function
function removeFromArray(arr, element) {
  for (var i = arr.length - 1; i >= 0; i--) {
    if (arr[i] == element) {
      arr.splice(i, 1);
    }
  }
}

//Fonction permettant de calculer l'heuristique
// @param: a, b pixels type Spot
// @return: d distance euclidienne
function heuristic(a, b) {
  var d = dist(a.i, a.j, b.i, b.j);  // Calcul de la distance euclidienne
  return d;
}

function dist(x1, y1, x2, y2) {
  return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


function vectorize(p) {
  let prevDiff = new vec2(0, 0);
  let currentVec = new vec2(0, 0);
  let vectors = [];

  for (let i = p.length - 2; i >= 0; i--) {
    let vecfrom = new vec2(p[i + 1].i, p[i + 1].j);
    let vecto = new vec2(p[i].i, p[i].j);

    let diff = vecto.sub(vecfrom);

    if (diff.isEqual(prevDiff) || prevDiff.isNull() || currentVec.isNull()) {
      currentVec = currentVec.add(diff);
    }
    else {
      vectors.push(currentVec);
      currentVec = diff;
    }
    prevDiff = diff;
  }
  currentVec.print();
  vectors.push(currentVec);

  return vectors;
}

function cloneGrid(c, r) {
  let newG = new Array(c);

  for (i = 0; i < cols; i++) {
    newG[i] = new Array(r);
  }

  for (i = 0; i < c; i++) {
    for (j = 0; j < r; j++) {
      newG[i][j] = new Spot(i, j);
      newG[i][j].wall = grid[i][j].wall;
    }
  }

  return newG;
}

function updateGrid(clone, c, r) {
  for (i = 0; i < c; i++) {
    for (j = 0; j < r; j++) {
      grid[i][j].wall = clone[i][j].wall;
    }
  }
}

function erode(c, r) {
  grid_temp = cloneGrid(c, r);

  for (i = 4; i < c - 4; i++) {
    for (j = 4; j < r - 4; j++) {
      if (grid[i][j + 1].wall == true || grid[i][j - 1].wall == true || grid[i + 1][j].wall == true || grid[i - 1][j].wall == true) {
        grid_temp[i][j].wall = true;
      }
    }
  }
  updateGrid(grid_temp, c, r);
}

function lineW(xi, yi, xf, yf) {
  //console.log("### Line ###");
  let l = new vec2(xf - xi, yf - yi);
  let N = 100;
  for (let i = 0; i < N; i++) {
    let x = Math.floor(xi + l.mul(i / N).x);
    let y = Math.floor(yi + l.mul(i / N).y);
    //console.log(x, y)
    grid[x][y].wall = true;
  }
}

function init(xi, yi, xf, yf) {

  console.log("from : (" + xi + ", " + yi + ")");
  console.log("to : (" + xf + ", " + yf + ")");
  // Depart
  x_init = xi;
  y_init = yi;

  x_fin = xf;
  y_fin = yf;

  start = grid[x_init / w + offsetX][offsetY - y_init / h]; // On donne le point de départ du robot
  end = grid[x_fin / w + offsetX][offsetY - y_fin / h]; // On donne le point d'arrivée du robot

  // On garantie que le point de départ et d'arrivée ne sont pas des obstacles
  start.wall = false;
  end.wall = false;

  openSet.push(start);    // Initialisation de la liste des noeuds à traiter

  toDraw();
}


offsetX = 0;
offsetY = 0;
function create_grid(terrain) {

  /** Extract JSON */

  // Forme du terrain
  let minX = 0, maxX = 0, minY = 0, maxY = 0;
  for (let P of terrain.terrain_evolution) {
    //console.log(P);
    if (P.point.x > maxX) maxX = P.point.x;
    if (P.point.x < minX) minX = P.point.x;
    if (P.point.y > maxY) maxY = P.point.y;
    if (P.point.y < minY) minY = P.point.y;
  }


  w = 6.25;
  h = 6.25;

  cols = (maxX - minX) / w;
  rows = (maxY - minY) / h;

  console.log("cols : " + cols);
  console.log("rows : " + rows);

  // Création des colones
  for (let i = 0; i < cols; i++) {
    grid[i] = new Array(rows);
  }

  // Création de noeuds dans la grille
  for (let i = 0; i < cols; i++) {
    for (let j = 0; j < rows; j++) {
      grid[i][j] = new Spot(i, j);
    }
  }

  // recherche des voisins pour chaque noeud de la grille
  for (let i = 0; i < cols; i++) {
    for (let j = 0; j < rows; j++) {
      grid[i][j].addNeighbors(grid);
    }
  }

  // Mise en place du terrain
  offsetX = - minX / w - 1;
  offsetY = (maxY - minY) / h - 1;

  grid[offsetX][offsetY].wall = true; // origine

  // Contour
  for (let i = 0; i < terrain.terrain_evolution.length; i++) {
    xi = Math.abs(terrain.terrain_evolution[(i + terrain.terrain_evolution.length - 1) % terrain.terrain_evolution.length].point.x / w + offsetX);
    yi = Math.abs(offsetY - terrain.terrain_evolution[(i + terrain.terrain_evolution.length - 1) % terrain.terrain_evolution.length].point.y / h);
    xf = Math.abs(terrain.terrain_evolution[i].point.x / w + offsetX);
    yf = Math.abs(offsetY - terrain.terrain_evolution[i].point.y / h);
    lineW(xi, yi, xf, yf);
  }

  // Obstacles
  for (let obstacle of terrain.obstacles) {
    if (obstacle.type == "polygone") {
      for (let i = 0; i < obstacle.coordonnees.length; i++) {

        xi = obstacle.coordonnees[(i + obstacle.coordonnees.length - 1) % obstacle.coordonnees.length].point.x / w + offsetX;
        yi = offsetY - obstacle.coordonnees[(i + obstacle.coordonnees.length - 1) % obstacle.coordonnees.length].point.y / h;
        xf = obstacle.coordonnees[i].point.x / w + offsetX;
        yf = offsetY - obstacle.coordonnees[i].point.y / h;

        lineW(xi, yi, xf, yf);
      }
    }

    if (obstacle.type == "cercle") {
      x = obstacle.coordonnees[0].centre.x / w + offsetX;
      y = offsetY - obstacle.coordonnees[0].centre.y / h;
      r = Math.floor(obstacle.coordonnees[1].rayon / h);
      //console.log(" R : " + r);
      lineW(x - r, y - r, x - r, y + r);
      lineW(x - r, y + r, x + r, y + r);
      lineW(x + r, y + r, x + r, y - r);
      lineW(x + r, y - r, x - r, y - r);
    }
  }

  // Erosion
  erode(cols, rows);
  erode(cols, rows);
}

let vec = [];
function toDraw() {
  loop = true;
  while (loop == true) {

    // On continue l'algorithme tant qu'il y'a des noeuds à traiter
    if (openSet.length > 0) {
      let l_Index = 0;    // Index du noeud de poids le plus faible

      // On parcourt la liste des noeuds à traiter
      for (var i = 0; i < openSet.length; i++) {

        // On vérifie si le poids du noeud traité est inférieur au poids minimum et on             
        // remplace dans ce cas le poids minimum
        if (openSet[i].f < openSet[l_Index].f) {
          l_Index = i;
        }
      }

      var current = openSet[l_Index]; // On se place au noeud de poids le plus faible

      // On vérifie si ce noeud correspond à la fin
      if (current === end) {

        loop = false;
        openSet.length = 0;
        console.log("Arrivé :)!");
        //console.log(path);

        // Vectorise le chemin
        vec = vectorize(path);
        //console.log(vec);
      }

      removeFromArray(openSet, current);  // On retire le noeud une fois traité

      // Un noeud traité ne peut etre retraité plus tard, il est envoyé
      // dans la liste des noeuds qui ne sont plus des étapes possible dans la suite du déplacement
      closedSet.push(current);

      var neighbors = current.neighbors;  // On simplifie l'écriture des voisins

      // On parcourt tous les voisins du noeuds auquel on se situe
      for (var i = 0; i < neighbors.length; i++) {

        var neighbor = neighbors[i];  // On simplifie l'écriture de chaque voisin

        // On vérifie si le voisin n'a pas déja été traité ou si le voisin n'est pas un obstacle
        if (!closedSet.includes(neighbor) && !neighbor.wall) {

          var tempG = current.g + 1;  // On actualise la valeur du g pour le noeud auquel on se situe
          var newPath = false;    // Varible vérifiant si le noeud voisin fera partie du chemin ou non

          // Si le voisin fait partie des noeuds à traiter
          if (openSet.includes(neighbor)) {
            // On vérifie si la valeur de g du voisin est inférieure et on actualise la valeur de g globale.
            // C'est donc un noeud possible pour arriver à la fin
            if (tempG < neighbor.g) {
              neighbor.g = tempG;
              newPath = true;
            }
          } else {
            neighbor.g = tempG;
            newPath = true;
            openSet.push(neighbor);
          }

          // On calcule le h et le f du noeud si celui ci est sur une trajectoire possible
          if (newPath) {
            neighbor.h = heuristic(neighbor, end);
            neighbor.f = neighbor.g + neighbor.h;
            neighbor.previous = current;
          }
        }
      }
      // Si il n'y a plus de noeuds à traiter et que l'on a pas atteint le point d'arrivée (aucun chemin possible)    
    } else {
      console.log('No Solution :(');
      loop = false;
      openSet.length = 0;
      return;

    }

    // On retient le chemin parcourut jusqu'ici:
    path = [];
    path.push(end);
    var temp = current;
    path.push(temp);

    while (temp.previous) {
      path.push(temp.previous);
      temp = temp.previous;
    }

  }
}


/*** Classes ***/

// Constructeur d'un spot (position dans la surface de déplacement):
class Spot {
  constructor(i, j) {
    /** Attributs **/
    // Coordonnées dans le tableau:
    this.i = i; this.j = j;

    // Fonction de poids de chaque noeud:
    this.f = 0; this.g = 0; this.h = 0;

    // Tableau des voisins de chaque noeud:
    this.neighbors = [];

    // Noeud précédemment traité:
    this.previous = undefined;
    this.wall = false; // détermine si le noeud est un obstacle

    // Fonction permettant d'ajouter des noeuds voisins
    // @param: grid tableau d'origine
    // @return void
    this.addNeighbors = function (grid) {
      let i = this.i;
      let j = this.j;

      // Vérification de la présence de voisin à droite
      if (i < cols - 1) { this.neighbors.push(grid[i + 1][j]); }

      // Vérification de la présence de voisin à gauche
      if (i > 0) { this.neighbors.push(grid[i - 1][j]); }

      // Vérification de la présence de voisin en haut        
      if (j < rows - 1) { this.neighbors.push(grid[i][j + 1]); }

      // Vérification de la présence de voisin en bas 
      if (j > 0) { this.neighbors.push(grid[i][j - 1]); }

      // Vérification de la présence de voisin en haut a gauche
      if (i > 0 && j > 0) { this.neighbors.push(grid[i - 1][j - 1]); }

      // Vérification de la présence de voisin en bas a gauche
      if (i < cols - 1 && j > 0) { this.neighbors.push(grid[i + 1][j - 1]); }

      // Vérification de la présence de voisin en bas a droite
      if (i < cols - 1 && j < rows - 1) { this.neighbors.push(grid[i + 1][j + 1]); }

      // Vérification de la présence de voisin en haut a droite
      if (i > 0 && j < rows - 1) { this.neighbors.push(grid[i - 1][j + 1]); }

    };
  }
}

// Classe vec2:
class vec2 {
  constructor(x, y) {
    this.x = x;
    this.y = y;

    // Somme de 2 vecteurs:
    this.add = function (v) { return new vec2(this.x + v.x, this.y + v.y); };

    // Difference entre 2 vecteurs:
    this.sub = function (v) { return new vec2(this.x - v.x, this.y - v.y); };

    // Multiplie un vecteur par un scalaire:
    this.mul = function (s) { return new vec2(this.x * s, this.y * s); };

    // Applique round:
    this.r = function () { return new vec2(Math.round(this.x), Math.round(this.y)); };

    // Test l'égalité entre 2 vecteurs:
    this.isEqual = function (v) { return (this.x == v.x) && (this.y == v.y); };

    // Affiche le vecteur:
    this.print = function () { console.log("(" + this.x + ", " + this.y + ")"); };

    // Test si le vecteur est nul:
    this.isNull = function () { return (this.x == 0) && (this.y == 0); };
  }
}
