# 🎮 Test de Tous les PowerUps - R-Type

## 🚀 Démarrage Rapide

```bash
./test_powerups.sh
```

Ce script va:
1. ✅ Vérifier que tous les fichiers nécessaires existent
2. 🔧 Compiler le projet
3. 🎮 Lancer le jeu avec le niveau de test `test_all_powerups.json`

---

## 📋 PowerUps Testés

| Type | Archetype | Effet | Sprite |
|------|-----------|-------|--------|
| **HEAL** | `heal_pickup_small` | +25 HP | r-typesheet30.gif (vert) |
| **ADD_SCORE** | `score_pickup_bronze` | +500 points | r-typesheet31.gif (jaune) |
| **CHANGE_WEAPON** | `spreadshot_pickup` | Spread Gun (3 balles) | r-typesheet33.gif (orange) |
| **CHANGE_WEAPON** | `laser_pickup` | Laser Gun | r-typesheet32.gif (bleu) |

---

## 🎯 Timeline du Niveau (45 secondes)

### Phase 1: HEAL (0-10s) ❤️
- **2s, 4s, 6s**: 3 pickups de heal verts
- **3s, 5s**: Ennemis pour perdre de la vie
- **Test**: Vérifier que HP augmente de +25 à chaque pickup

### Phase 2: SCORE (10-20s) ⭐
- **11s, 13s, 15s, 17s, 19s**: 5 pickups de score jaunes
- **Test**: Vérifier que le score augmente de +500 (total: 2500)

### Phase 3: SPREAD GUN (20-30s) 🔫
- **21s**: Pickup orange (Spread Gun)
- **23-26s**: Ennemis pour tester l'arme
- **Test**: Vérifier que chaque tir lance 3 balles en éventail

### Phase 4: LASER GUN (30-40s) ⚡
- **31s**: Pickup bleu (Laser)
- **33-36s**: Ennemis pour tester le laser
- **Test**: Vérifier le tir laser continu

### Phase 5: MIX (40-45s) 🎁
- **41s**: Les 4 types de powerups en même temps!
- **43-44s**: Vague finale d'ennemis
- **Test**: Vérifier que tous fonctionnent ensemble

---

## 📊 Résultats Attendus

Après 45 secondes de jeu:
- ✅ **HP**: ~75-100 (selon dégâts pris)
- ✅ **Score**: 2500+ (5 pickups de score)
- ✅ **Arme**: Laser Gun (dernier pickup)
- ✅ **PowerUps ramassés**: 12 au total

---

## 🔍 Checklist de Validation

### HEAL ❤️
- [ ] Sprite vert apparaît
- [ ] HP augmente de +25 au contact
- [ ] Message console: "Player healed by 25 HP"
- [ ] HP ne dépasse pas max_hp (100)

### ADD_SCORE ⭐
- [ ] Sprite jaune apparaît
- [ ] Score augmente de +500 au contact
- [ ] Message console: "Score increased by 500"
- [ ] Score affiché à l'écran

### SPREAD GUN 🔫
- [ ] Sprite orange apparaît
- [ ] Changement d'arme au contact
- [ ] Message console: "Weapon changed to spread_gun"
- [ ] Tir en éventail (3 balles)

### LASER GUN ⚡
- [ ] Sprite bleu apparaît
- [ ] Changement d'arme au contact
- [ ] Message console: "Weapon changed to laser_gun"
- [ ] Laser continu (pas de projectiles séparés)

---

## 📄 Documentation Complète

Pour un guide détaillé avec tests avancés:
👉 **[docs/Test_ALL_PowerUps_Guide.md](docs/Test_ALL_PowerUps_Guide.md)**

---

## 🐛 Troubleshooting

### Le niveau ne charge pas
```bash
# Vérifier que le fichier existe
ls -la Config/levels/test_all_powerups.json
```

### Les powerups n'apparaissent pas
```bash
# Vérifier les archetypes
grep -A 10 "powerups" Config/archetypes.json
```

### Compilation échoue
```bash
# Utiliser le script de build standard
./kev_test.sh
```

---

## 🔄 Revenir au niveau normal

Pour tester les armes uniquement:
```bash
# Éditer All_src/scenes/TestGameScene.cpp, ligne ~52
levelManager.load("Config/levels/test_weapons.json");
```

Pour le niveau complet:
```bash
levelManager.load("Config/levels/level_1.json");
```

---

## 🎨 Customisation

Pour modifier le niveau de test, éditez:
```
Config/levels/test_all_powerups.json
```

Structure d'un événement:
```json
{
  "time": 5.0,
  "spawn": "powerups.heal_pickup_small",
  "position": { "x": 1300, "y": 300 }
}
```

---

## 📝 Rapport de Bug

Si vous trouvez un bug, notez:
1. **PowerUp affecté**: HEAL / SCORE / CHANGE_WEAPON
2. **Comportement attendu**: ...
3. **Comportement observé**: ...
4. **Étapes pour reproduire**: ...

---

**Bon test! 🚀**
