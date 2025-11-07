# Snapshot System Documentation

## Vue d'ensemble

Le système de snapshot permet de sérialiser l'état complet des entités du jeu pour les envoyer via le réseau aux clients. Il utilise une quantification pour optimiser la taille des paquets.

## Structure des données

### Snapshot
```cpp
struct Snapshot {
    uint32_t tick;                          // Tick du jeu (4 bytes)
    std::vector<EntitySnapshot> entities;   // Liste des entités
};
```

### EntitySnapshot
```cpp
struct EntitySnapshot {
    uint32_t id;                           // ID de l'entité (4 bytes)
    std::vector<Components> components;    // Liste des composants
    uint8_t component_count;               // Nombre de composants (1 byte)
};
```

### Components
Union contenant tous les types de composants possibles avec leur représentation sérialisée (int16_t quantifiés).

## Utilisation

### 1. Créer un snapshot pour une seule entité

```cpp
#include "Server/include/SnapshotHelper.hpp"

Entity player = registry.spawn_entity();
// ... ajouter des composants au player ...

// Créer le snapshot
EntitySnapshot playerSnapshot = SnapshotHelper::createEntitySnapshot(player, registry);
```

### 2. Créer un snapshot pour plusieurs entités

```cpp
std::vector<Entity> entities = {player1, player2, enemy1};
Snapshot snapshot = SnapshotHelper::createSnapshot(entities, registry, currentTick);
```

### 3. Sérialiser le snapshot pour l'envoi réseau

```cpp
Snapshot snapshot = SnapshotHelper::createSnapshot(entities, registry, currentTick);
std::vector<uint8_t> data = snapshot.serialize();

// Envoyer via UDP
sendToClient(data.data(), data.size());
```

### 4. Exemple complet dans OnlineGameScene_server

```cpp
Snapshot OnlineGameScene_server::init_entity_player(uint32_t player_id) {
    // Créer le joueur
    Entity player = gameSceneEngine.getRegistry().spawn_entity();
    player.set_name("Player" + std::to_string(player_id));
    
    // Ajouter les composants
    gameSceneEngine.getRegistry().add_component(player, Component::Core::Position(100.0f, 300.0f));
    gameSceneEngine.getRegistry().add_component(player, Component::Core::Velocity(0.0f, 0.0f));
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::PlayerTag(player_id));
    // ... autres composants ...
    
    // Créer la barre de vie
    Entity health_bar = gameSceneEngine.getRegistry().spawn_entity();
    // ... ajouter composants de la barre de vie ...
    
    // Créer le snapshot automatiquement
    std::vector<Entity> entities = {player, health_bar};
    return SnapshotHelper::createSnapshot(entities, gameSceneEngine.getRegistry(), 0);
}
```

## Types de composants supportés

| ComponentType | Taille sérialisée | Description |
|--------------|------------------|-------------|
| POSITION | 4 bytes | Position x, y (quantifiées) |
| VELOCITY | 4 bytes | Vélocité dx, dy (quantifiées) |
| SCALE | 4 bytes | Échelle width, height (quantifiées) |
| ROTATION | 2 bytes | Angle de rotation (quantifié) |
| HITBOX | 4 bytes | Dimensions width, height (quantifiées) |
| SPRITE | 8 bytes | texture_id, z_index, frame_width, frame_height |
| ANIMATION | 8 bytes | start_frame, num_frames, frame_duration, loops |
| HEALTH | 2 bytes | Points de vie (hp) |
| HEALTH_BAR | 6 bytes | owner, offset_x, offset_y |
| RECTANGLE_SHAPE | 8 bytes | Couleur RGBA |
| CIRCLE_SHAPE | 8 bytes | radius, outline_thickness |
| PLAYER_TAG | 2 bytes | ID du joueur |
| DAMAGE | 2 bytes | Montant des dégâts |
| ENEMY_TAG | 2 bytes | Type d'ennemi |
| MISSILE_TAG | 2 bytes | ID du propriétaire |
| SCORE | 2 bytes | Valeur du score |
| DEAD | 2 bytes | Marqueur de mort |
| CONTROLLABLE | 0 bytes | Pas de données |

## Quantification

Le système utilise un facteur de quantification de **10.0** pour convertir les valeurs flottantes en int16_t :

```cpp
constexpr float QUANTIZATION_FACTOR = 10.0f;  // Précision de 0.1
```

### Exemple :
- Position (100.5, 200.3) → int16_t (1005, 2003)
- Après déquantification : (100.5, 200.3)

## Format de sérialisation

```
[Header]
  - tick (4 bytes)
  - entity_count (2 bytes)
  
[For each entity]
  - entity_id (4 bytes)
  - component_count (1 byte)
  
  [For each component]
    - component_type (1 byte)
    - component_data (variable, selon le type)
```

## Avantages

1. ✅ **Automatique** : Sérialisation automatique de tous les composants d'une entité
2. ✅ **Optimisé** : Quantification pour réduire la bande passante
3. ✅ **Extensible** : Facile d'ajouter de nouveaux types de composants
4. ✅ **Type-safe** : Conversion typée pour chaque composant
5. ✅ **Little-endian** : Compatible réseau standardisé

## Améliorations futures

- [ ] Delta compression (envoyer seulement ce qui a changé)
- [ ] Prioritisation des entités (envoyer les plus importantes d'abord)
- [ ] Interpolation côté client pour smooth rendering
- [ ] Compression avec zlib/lz4 pour gros snapshots

## Fichiers concernés

- `Server/include/Snapshot.hpp` - Définitions des structures
- `Server/src/Snapshot.cpp` - Implémentation de serialize() et calculateSize()
- `Server/include/SnapshotHelper.hpp` - Helper pour créer snapshots depuis entities
- `Server/src/SnapshotHelper.cpp` - Implémentation des conversions
- `All_src/scenes/Online_GameScene_server.cpp` - Utilisation dans le jeu
