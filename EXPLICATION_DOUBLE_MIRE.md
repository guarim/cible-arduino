# 🎯 Système de Double Mire - Explication Technique

## Concept du système

Le système utilise **deux mires** pour réaliser un asservissement visuel :

### 🔵 Mire Bleue (CENTRE)
- **Position** : Fixe au centre du canvas (320, 240)
- **Rôle** : Point de référence, objectif à atteindre
- **Couleur** : Bleu (#0088FF)
- **Signification** : C'est la position que la caméra doit viser

### 🟢 Mire Verte (CIBLE)
- **Position** : Sur le visage détecté (entre les yeux, au-dessus des sourcils)
- **Rôle** : Cible mobile à suivre
- **Couleur** : Vert (#00FF00)
- **Signification** : C'est le visage détecté en temps réel

## Principe de fonctionnement

### 1. Objectif du système
**Faire coïncider les deux mires** : Les servomoteurs doivent orienter la caméra de façon à ce que la mire verte (visage) se déplace vers la mire bleue (centre).

### 2. Calcul de l'erreur
```
Erreur X = Position Cible X - Position Centre X
Erreur Y = Position Cible Y - Position Centre Y
```

**Exemple** :
- Centre : (320, 240)
- Visage détecté : (400, 200)
- Erreur X : +80 pixels (la cible est à droite)
- Erreur Y : -40 pixels (la cible est en haut)

### 3. Calcul des angles des servos

#### Servo X (Horizontal - Broche 6)
```
Angle X = 90° + (Erreur X / Largeur) × 180°
```

**Logique** :
- Si la cible est **à droite** du centre (Erreur X > 0) → Angle > 90° → Le servo tourne à droite
- Si la cible est **à gauche** du centre (Erreur X < 0) → Angle < 90° → Le servo tourne à gauche
- Si la cible est **au centre** (Erreur X = 0) → Angle = 90° → Position neutre

#### Servo Y (Vertical - Broche 9)
```
Angle Y = 90° - (Erreur Y / Hauteur) × 180°
```

**Logique** :
- Si la cible est **en haut** du centre (Erreur Y < 0) → Angle > 90° → Le servo tourne vers le haut
- Si la cible est **en bas** du centre (Erreur Y > 0) → Angle < 90° → Le servo tourne vers le bas
- Si la cible est **au centre** (Erreur Y = 0) → Angle = 90° → Position neutre

**Note** : Le signe négatif pour Y est dû à l'inversion du système de coordonnées (Y augmente vers le bas dans l'image)

### 4. Résultat attendu

Quand le système fonctionne correctement :
1. **La caméra détecte un visage** → Mire verte apparaît
2. **Le système calcule l'écart** entre les deux mires
3. **Les servos s'orientent** pour réduire cet écart
4. **La caméra bouge** et la mire verte se rapproche de la mire bleue
5. **Les deux mires se superposent** → Le visage est centré !

## Indicateurs visuels

### Ligne pointillée jaune
Une ligne relie les deux mires pour visualiser l'écart à corriger.

### Distance d'erreur
Affichée dans l'interface avec un code couleur :
- 🟢 **< 20 pixels** : Excellent centrage
- 🟠 **20-50 pixels** : Centrage moyen
- 🔴 **> 50 pixels** : Centrage imprécis

### Formule de la distance
```
Distance = √(Erreur X² + Erreur Y²)
```

## Comportement attendu

### Scénario 1 : Visage à droite du centre
```
État initial:
- Mire bleue : Centre (320, 240)
- Mire verte : Droite (450, 240)
- Erreur X : +130 pixels

Action du servo X:
- Calcul : 90° + (130/640) × 180° ≈ 127°
- Le servo tourne à droite (127° > 90°)
- La caméra pivote à droite
- Le visage se déplace vers la gauche dans l'image
- La mire verte se rapproche de la mire bleue
```

### Scénario 2 : Visage en haut à gauche
```
État initial:
- Mire bleue : Centre (320, 240)
- Mire verte : Haut-gauche (200, 150)
- Erreur X : -120 pixels
- Erreur Y : -90 pixels

Action des servos:
- Servo X : 90° + (-120/640) × 180° ≈ 56°
  → Tourne à gauche (56° < 90°)
- Servo Y : 90° - (-90/480) × 180° ≈ 124°
  → Tourne vers le haut (124° > 90°)

Résultat:
- La caméra pivote vers le haut et la gauche
- Le visage se centre progressivement
```

### Scénario 3 : Visage centré (objectif atteint)
```
État:
- Mire bleue : (320, 240)
- Mire verte : (320, 240)
- Erreur : 0 pixels
- Distance : 0 px 🟢

Angles des servos:
- Servo X : 90° (neutre)
- Servo Y : 90° (neutre)
- Les deux mires sont superposées ✓
```

## Lissage des mouvements

Pour éviter les mouvements brusques, un **lissage exponentiel** est appliqué :

```javascript
Angle lissé = Angle actuel + (Angle cible - Angle actuel) × 0.3
```

**Avantage** : Mouvements fluides et progressifs, pas de vibrations.

## Vérification du bon fonctionnement

### ✅ Checklist
- [ ] La mire bleue est visible au centre du canvas
- [ ] La mire verte apparaît sur votre visage
- [ ] Une ligne jaune relie les deux mires
- [ ] L'erreur de centrage s'affiche
- [ ] Les angles des servos changent quand vous bougez
- [ ] Les servos bougent physiquement
- [ ] Le visage se centre progressivement

### 🎯 Test de précision
1. Placez-vous face à la caméra
2. Bougez lentement vers la droite
3. **Comportement attendu** :
   - La mire verte se déplace à droite
   - Le servo X augmente (> 90°)
   - La caméra pivote à droite
   - Votre visage reste visible et se recentre

## Calibration

Si le système ne se comporte pas comme attendu, vérifiez :

1. **Montage mécanique** : Les servos sont-ils bien fixés ?
2. **Sens de rotation** : Les servos tournent-ils dans le bon sens ?
3. **Angles limites** : Les servos peuvent-ils aller de 0° à 180° ?
4. **Connexions** : Broche 6 = X (horizontal), Broche 9 = Y (vertical)

### Inversion des axes si nécessaire

Si un servo tourne dans le mauvais sens, modifiez dans le code HTML :

```javascript
// Pour inverser l'axe X
const servoX = SERVO_CENTER - (errorX / VIDEO_WIDTH) * (SERVO_MAX - SERVO_MIN);

// Pour inverser l'axe Y
const servoY = SERVO_CENTER + (errorY / VIDEO_HEIGHT) * (SERVO_MAX - SERVO_MIN);
```

## Avantages du système à double mire

✅ **Visuel et intuitif** : On voit immédiatement l'écart à corriger
✅ **Référence fixe** : La mire bleue ne bouge jamais
✅ **Feedback en temps réel** : La distance d'erreur indique la précision
✅ **Asservissement visuel** : Principe classique de robotique
✅ **Facilite le debug** : On voit tout ce qui se passe

## Améliorations possibles

1. **PID Controller** : Remplacer le calcul proportionnel simple par un contrôleur PID
2. **Zones mortes** : Ne pas bouger si l'erreur est < 10 pixels
3. **Vitesse adaptative** : Bouger vite si l'erreur est grande, lentement si petite
4. **Historique** : Tracer la trajectoire de la mire verte
5. **Multi-visages** : Suivre le visage le plus proche du centre

---

**En résumé** : Le système fait en sorte que les servomoteurs orientent la caméra pour que le visage détecté (mire verte) soit toujours au centre de l'image (mire bleue). C'est un asservissement visuel classique avec feedback en temps réel ! 🎯
