/*
 * Système de Suivi Facial - Code Arduino Uno
 * Contrôle de deux servomoteurs pour le suivi de visage
 * Servo X: Broche 6 (mouvement horizontal)
 * Servo Y: Broche 9 (mouvement vertical)
 */

#include <Servo.h>

// Déclaration des servos
Servo servoX;  // Servo horizontal (broche 6)
Servo servoY;  // Servo vertical (broche 9)

// Broches des servos
const int SERVO_X_PIN = 6;
const int SERVO_Y_PIN = 9;

// Limites des servos
const int SERVO_MIN = 0;
const int SERVO_MAX = 180;
const int SERVO_CENTER = 90;

// Variables pour stocker les positions
int targetX = SERVO_CENTER;
int targetY = SERVO_CENTER;
int currentX = SERVO_CENTER;
int currentY = SERVO_CENTER;

// Paramètres de lissage
const int SMOOTH_FACTOR = 5;  // Plus le chiffre est élevé, plus le mouvement est lissé

// Buffer pour la réception série
String inputString = "";
boolean stringComplete = false;

void setup() {
  // Initialisation de la communication série
  Serial.begin(9600);
  
  // Attachement des servos aux broches
  servoX.attach(SERVO_X_PIN);
  servoY.attach(SERVO_Y_PIN);
  
  // Position initiale au centre
  servoX.write(SERVO_CENTER);
  servoY.write(SERVO_CENTER);
  
  // Message de démarrage
  Serial.println("Système de suivi facial - Arduino Uno");
  Serial.println("Servo X: Broche 6");
  Serial.println("Servo Y: Broche 9");
  Serial.println("Prêt à recevoir les commandes...");
  
  // Réserve de mémoire pour la chaîne d'entrée
  inputString.reserve(50);
  
  delay(1000);
}

void loop() {
  // Traiter les données série si disponibles
  if (stringComplete) {
    parseCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
  
  // Déplacer les servos progressivement vers la cible
  smoothMove();
  
  delay(20);  // Petit délai pour stabiliser les mouvements
}

// Réception des données série
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

// Parser les commandes reçues
// Format attendu: "X:90,Y:90"
void parseCommand(String command) {
  command.trim();
  
  int xIndex = command.indexOf("X:");
  int yIndex = command.indexOf("Y:");
  int commaIndex = command.indexOf(",");
  
  if (xIndex != -1 && yIndex != -1 && commaIndex != -1) {
    // Extraction des valeurs
    String xValue = command.substring(xIndex + 2, commaIndex);
    String yValue = command.substring(yIndex + 2);
    
    // Conversion en entiers
    int newX = xValue.toInt();
    int newY = yValue.toInt();
    
    // Vérification des limites
    if (newX >= SERVO_MIN && newX <= SERVO_MAX) {
      targetX = newX;
    }
    
    if (newY >= SERVO_MIN && newY <= SERVO_MAX) {
      targetY = newY;
    }
    
    // Debug
    Serial.print("Cible reçue - X: ");
    Serial.print(targetX);
    Serial.print("° Y: ");
    Serial.print(targetY);
    Serial.println("°");
  }
}

// Déplacement progressif des servos (lissage)
void smoothMove() {
  bool moved = false;
  
  // Mouvement horizontal
  if (currentX != targetX) {
    if (currentX < targetX) {
      currentX = min(currentX + 1, targetX);
    } else {
      currentX = max(currentX - 1, targetX);
    }
    servoX.write(currentX);
    moved = true;
  }
  
  // Mouvement vertical
  if (currentY != targetY) {
    if (currentY < targetY) {
      currentY = min(currentY + 1, targetY);
    } else {
      currentY = max(currentY - 1, targetY);
    }
    servoY.write(currentY);
    moved = true;
  }
  
  // Afficher la position actuelle si un mouvement a eu lieu
  if (moved) {
    Serial.print("Position - X: ");
    Serial.print(currentX);
    Serial.print("° Y: ");
    Serial.print(currentY);
    Serial.println("°");
  }
}

// Fonction pour retourner à la position centrale
void returnToCenter() {
  targetX = SERVO_CENTER;
  targetY = SERVO_CENTER;
  
  while (currentX != targetX || currentY != targetY) {
    smoothMove();
    delay(20);
  }
  
  Serial.println("Position centrale atteinte");
}
