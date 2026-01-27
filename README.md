# Système de Suivi Facial Automatisé

## 📋 Description

Application web professionnelle de détection et suivi de visage en temps réel utilisant MediaPipe. Le système contrôle deux servomoteurs via Arduino Uno pour suivre automatiquement les mouvements du visage détecté.

## 🎯 Fonctionnalités

- ✅ Détection de visage en temps réel avec MediaPipe
- ✅ Affichage vidéo 640×480 pixels
- ✅ Mire de ciblage centrée entre les yeux et au-dessus des sourcils
- ✅ Calcul automatique des angles des servomoteurs (0-180°)
- ✅ Interface moderne et professionnelle
- ✅ Affichage en temps réel des coordonnées et angles
- ✅ Indicateur de statut (Cible acquise / Pas de cible)
- ✅ Mouvements lissés des servomoteurs
- ✅ Communication série avec Arduino

## 🔧 Matériel requis

### Arduino
- Arduino Uno
- 2 servomoteurs (0-180°)
- Câbles de connexion
- Alimentation externe pour les servos (recommandé)

### Connexions
- **Servo X (horizontal)** : Broche 6
- **Servo Y (vertical)** : Broche 9
- **GND** : Masse commune
- **5V** : Alimentation (ou source externe)

## 📦 Installation

### 1. Configuration Arduino

1. Ouvrir l'IDE Arduino
2. Charger le fichier `arduino_face_tracking.ino`
3. Installer la bibliothèque **Servo** si nécessaire :
   - Croquis → Inclure une bibliothèque → Gérer les bibliothèques
   - Rechercher "Servo"
   - Installer
4. Sélectionner la carte : Arduino Uno
5. Sélectionner le port série approprié
6. Téléverser le code

### 2. Configuration Web

1. Ouvrir le fichier `face_tracking.html` dans un navigateur moderne :
   - **Google Chrome** (recommandé)
   - Microsoft Edge
   - Firefox

2. Autoriser l'accès à la caméra lorsque demandé

3. Le système charge automatiquement MediaPipe depuis le CDN

## 🚀 Utilisation

### Démarrage simple

1. **Téléverser le code Arduino** sur la carte
2. **Ouvrir le fichier HTML** dans le navigateur
3. **Autoriser l'accès à la caméra**
4. Le système démarre automatiquement

### Interface

#### Zone gauche - Vidéo
- Affichage de la caméra en temps réel
- Mire de ciblage verte sur le visage détecté
- Format : 640×480 pixels

#### Zone droite - Coordonnées
- **Servo X (Pin 6)** : Angle horizontal (0-180°)
- **Servo Y (Pin 9)** : Angle vertical (0-180°)
- **Position X** : Coordonnée horizontale en pixels
- **Position Y** : Coordonnée verticale en pixels
- **Arduino** : Statut de la connexion

#### Zone inférieure - Statut
- 🟢 **CIBLE ACQUISE** : Visage détecté et suivi actif
- 🔴 **PAS DE CIBLE** : Aucun visage détecté

## 🔌 Communication Série (Web Serial API)

Pour activer la communication réelle avec l'Arduino :

### Option 1 : Utiliser Web Serial API (Chrome/Edge)

Modifier dans le fichier HTML la fonction `sendToArduino()` :

```javascript
// Variables globales
let port;
let writer;

// Fonction de connexion au port série
async function connectArduino() {
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        
        const textEncoder = new TextEncoderStream();
        const writableStreamClosed = textEncoder.readable.pipeTo(port.writable);
        writer = textEncoder.writable.getWriter();
        
        arduinoStatus.textContent = '⚡ Arduino: Connecté';
    } catch (error) {
        console.error('Erreur de connexion:', error);
        arduinoStatus.textContent = '⚠ Arduino: Erreur';
    }
}

// Modifier sendToArduino
async function sendToArduino(servoX, servoY) {
    const command = `X:${Math.round(servoX)},Y:${Math.round(servoY)}\n`;
    
    if (writer) {
        try {
            await writer.write(command);
        } catch (error) {
            console.error('Erreur d\'envoi:', error);
        }
    }
}
```

Ajouter un bouton de connexion dans le HTML :

```html
<button onclick="connectArduino()">Connecter Arduino</button>
```

### Option 2 : Utiliser un serveur Node.js (Alternative)

Créer un serveur WebSocket qui fait le pont entre le navigateur et Arduino via serial port.

## 🎨 Fonctionnement de la mire

La mire de ciblage est positionnée :
- ✅ **Horizontalement** : Centrée entre les deux yeux
- ✅ **Verticalement** : Au-dessus des sourcils (~30px au-dessus du centre des yeux)

Composition de la mire :
- Cercle extérieur
- Cercle intérieur
- Croix de visée
- Point central
- Effet lumineux vert

## 📊 Format des commandes Arduino

```
X:90,Y:90\n
```

- **X** : Angle du servo horizontal (0-180°)
- **Y** : Angle du servo vertical (0-180°)
- **\n** : Caractère de fin de ligne

## ⚙️ Configuration avancée

### Modifier la sensibilité

Dans le fichier HTML, ajuster :

```javascript
const SERVO_MIN = 0;      // Angle minimum
const SERVO_MAX = 180;    // Angle maximum
const SERVO_CENTER = 90;  // Position centrale
```

### Ajuster le lissage

Dans le code Arduino :

```cpp
const int SMOOTH_FACTOR = 5;  // Augmenter pour plus de lissage
```

Dans le code HTML :

```javascript
function smoothServo(current, target, factor = 0.3) {
    // factor: 0.1 = très lissé, 1.0 = aucun lissage
    return current + (target - current) * factor;
}
```

### Modifier la confiance de détection

```javascript
faceDetection.setOptions({
    model: 'short',
    minDetectionConfidence: 0.5  // 0.0 à 1.0
});
```

## 🐛 Dépannage

### La caméra ne s'affiche pas
- Vérifier les autorisations du navigateur
- Utiliser HTTPS ou localhost
- Vérifier qu'aucune autre application n'utilise la caméra

### Les servos ne bougent pas
- Vérifier les connexions (broches 6 et 9)
- Vérifier l'alimentation des servos
- Ouvrir le moniteur série (9600 bauds) pour voir les commandes

### Détection instable
- Améliorer l'éclairage
- Augmenter `minDetectionConfidence`
- Ajuster le facteur de lissage

### Communication série ne fonctionne pas
- Chrome/Edge uniquement pour Web Serial API
- Vérifier que le port série n'est pas utilisé par un autre programme
- Redémarrer le navigateur et l'Arduino

## 📝 Notes techniques

- **MediaPipe** : Utilise le modèle 'short' optimisé pour la détection rapide
- **Canvas** : Résolution native 640×480
- **Framerate** : Dépend de la caméra (généralement 30 FPS)
- **Latence** : ~50-100ms pour le suivi complet

## 🔒 Sécurité et confidentialité

- Aucune donnée vidéo n'est enregistrée
- Tout le traitement est local (navigateur)
- Pas de connexion à des serveurs externes (sauf CDN MediaPipe)
- Les images ne quittent jamais votre ordinateur

## 📄 Licence

Ce projet est fourni à des fins éducatives et de démonstration.

## 👨‍💻 Support

Pour toute question ou problème :
1. Vérifier la section Dépannage
2. Consulter la documentation MediaPipe
3. Vérifier les connexions Arduino

## 🎓 Améliorations possibles

- [ ] Ajout de filtres de Kalman pour un suivi plus précis
- [ ] Enregistrement des trajectoires
- [ ] Mode multi-visages
- [ ] Interface de calibration des servos
- [ ] Sauvegarde des paramètres
- [ ] Mode de démo automatique
- [ ] Détection de gestes
