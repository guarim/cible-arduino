# 🚀 Guide de Démarrage Rapide

## Installation en 5 minutes

### Étape 1: Préparation de l'Arduino (5 min)
```
1. Ouvrir Arduino IDE
2. Charger le fichier: arduino_face_tracking.ino
3. Brancher l'Arduino Uno en USB
4. Sélectionner la carte: Arduino Uno
5. Sélectionner le port COM
6. Cliquer sur "Téléverser" (→)
7. Attendre "Téléversement terminé"
8. FERMER le moniteur série et l'IDE Arduino
```

### Étape 2: Connexion du matériel (2 min)
```
Arduino Uno:
├── USB → Ordinateur (alimentation + communication)
├── Pin 6 → Servo X (signal)
├── Pin 9 → Servo Y (signal)
├── GND → Servos (masse commune)
└── 5V → Servos (ou alimentation externe recommandée)

Caméra USB:
└── USB → Ordinateur
```

### Étape 3: Lancement de l'application (2 min)
```
1. Ouvrir Google Chrome ou Microsoft Edge
2. Glisser-déposer face_tracking.html dans le navigateur
3. Autoriser l'accès à la caméra (popup)
4. Sélectionner la caméra USB dans le menu déroulant
5. Cliquer sur "🔌 Connecter Arduino"
6. Sélectionner le port COM de l'Arduino
7. Cliquer sur "Connecter"
```

### Étape 4: Test (1 min)
```
✓ Le statut affiche "Arduino connecté" avec un point vert
✓ Vous voyez votre visage dans le canvas
✓ Une mire verte apparaît sur votre visage
✓ Le message "CIBLE ACQUISE" s'affiche
✓ Les servos bougent quand vous bougez votre tête
✓ Les angles s'affichent en temps réel (0-180°)
```

## Troubleshooting Express

### ❌ "Impossible de se connecter à l'Arduino"
➡️ Fermez l'IDE Arduino complètement

### ❌ "Caméra USB non détectée"
➡️ Débranchez/rebranchez la caméra USB puis rafraîchissez (F5)

### ❌ "Les servos ne bougent pas"
➡️ Vérifiez les connexions des broches 6 et 9

### ❌ "Web Serial API non disponible"
➡️ Utilisez Chrome ou Edge (pas Firefox/Safari)

### ❌ "Pas de mire verte sur le visage"
➡️ Améliorez l'éclairage de la pièce

## Commandes Arduino (pour test manuel)

Si vous voulez tester l'Arduino sans l'application web:

1. Ouvrir le moniteur série (9600 bauds)
2. Taper ces commandes:

```
X:90,Y:90   → Centre (position neutre)
X:0,Y:90    → Gauche
X:180,Y:90  → Droite
X:90,Y:0    → Haut
X:90,Y:180  → Bas
X:45,Y:45   → Coin supérieur gauche
```

## Schéma de connexion simple

```
                    ┌─────────────┐
                    │  ORDINATEUR │
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │ USB        │        USB │
              ▼            ▼            │
      ┌──────────┐   ┌──────────┐      │
      │  ARDUINO │   │  CAMÉRA  │      │
      │   UNO    │   │   USB    │      │
      └─┬──┬──┬──┘   └──────────┘      │
        │  │  │                         │
   Pin6 │  │  │ Pin9                    │
        │  │  │                         │
        ▼  │  ▼                         │
    ┌────┐│┌────┐                       │
    │    │││    │                       │
    │ X  ││ Y  │ ← Servomoteurs        │
    │    ││    │                        │
    └────┘│└────┘                       │
          │                             │
        GND (masse commune)             │
```

## Indicateurs de l'interface

### Statut de connexion
- 🟢 Point vert + "Arduino connecté" = ✅ Tout fonctionne
- 🔴 Point rouge + "Arduino non connecté" = ⚠️ Connecter l'Arduino

### Statut de détection
- 🟢 "CIBLE ACQUISE" (vert clignotant) = Visage détecté
- 🔴 "PAS DE CIBLE" (rouge) = Aucun visage détecté

### Coordonnées affichées
- **Servo X**: 0° (gauche) à 180° (droite)
- **Servo Y**: 0° (haut) à 180° (bas)
- **Position X/Y**: Position en pixels dans l'image

## Support

En cas de problème:
1. Consultez le fichier README.md complet
2. Vérifiez la section "Dépannage"
3. Testez chaque composant séparément (caméra, Arduino, servos)

## Configuration avancée

Pour personnaliser le système:
- **Vitesse de suivi**: Modifier `factor` dans `smoothServo()` (HTML)
- **Vitesse servos**: Modifier `SMOOTH_FACTOR` (Arduino)
- **Sensibilité détection**: Modifier `minDetectionConfidence` (HTML)
- **Limites servos**: Modifier `SERVO_MIN` et `SERVO_MAX` (HTML et Arduino)

---
**Bon suivi facial! 🎯**
