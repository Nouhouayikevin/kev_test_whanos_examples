# R-Type Server (Skeleton)

## 📌 Description
Ce projet est un **squelette d’architecture serveur** pour un jeu type R-Type, basé sur un protocole de communication donné.

Le serveur gère :
- **TCP** pour le login et l’attribution d’un `playerId`.
- **UDP** pour le jeu en temps réel (inputs, mises à jour d’entités).
- Un **monde** avec des entités, mis à jour par des systèmes (mouvement, tir, collision, spawn).
- La **sérialisation** et **désérialisation** des messages selon la fiche protocole.

## 📂 Structure du projet
- `include/` : headers (.hpp)
- `src/` : sources (.cpp)
- `main.cpp` : point d’entrée du serveur
- `CMakeLists.txt` : pour compiler avec CMake
- `README.md` : ce fichier

## ⚙️ Compilation
```bash
mkdir build && cd build
cmake ..
make
```

## ▶️ Exécution
```bash
./rtype_server
```

## 🔮 Étapes suivantes
- Implémenter la logique réelle des systèmes (mouvement, tir, collisions).
- Compléter `Protocol.cpp` pour gérer tous les opcodes.
- Ajouter un client pour tester la communication.
