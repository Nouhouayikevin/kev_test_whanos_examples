/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SystemAutoRegister.hpp - Auto-registration magique des systèmes
*/

#pragma once

#include "SystemFactory.hpp"

/**
 * @brief Macro pour auto-enregistrer un système dans la SystemFactory
 * 
 * Cette macro doit être placée DANS LA CLASSE du système (section private ou public).
 * Elle crée une variable statique qui s'initialise automatiquement au chargement du programme,
 * enregistrant ainsi le système dans la factory SANS TOUCHER AU MOTEUR.
 * 
 * @param SystemClass Le nom de la classe du système (ex: PlayerControlSystem)
 * @param SystemName Le nom sous forme de string pour le JSON (ex: "PlayerControlSystem")
 * 
 * ╔════════════════════════════════════════════════════════════════╗
 * ║  UTILISATION :                                                  ║
 * ╠════════════════════════════════════════════════════════════════╣
 * ║  class MySystem : public ISystem {                             ║
 * ║  public:                                                       ║
 * ║      MySystem(std::string group) { ... }                       ║
 * ║      void run(GameEngine& e, float dt) override { ... }        ║
 * ║                                                                ║
 * ║      // ✨ LIGNE MAGIQUE (à ajouter)                           ║
 * ║      AUTO_REGISTER_SYSTEM(MySystem, "MySystem")                ║
 * ║  };                                                            ║
 * ╚════════════════════════════════════════════════════════════════╝
 * 
 * ╔════════════════════════════════════════════════════════════════╗
 * ║  COMMENT ÇA MARCHE :                                           ║
 * ╠════════════════════════════════════════════════════════════════╣
 * ║  1. La macro crée une variable statique inline                ║
 * ║  2. Cette variable s'initialise avec une lambda               ║
 * ║  3. La lambda enregistre le système dans la factory           ║
 * ║  4. Tout ça se passe AVANT main(), automatiquement !          ║
 * ║                                                                ║
 * ║  Résultat : Le système est disponible dans la factory         ║
 * ║             dès le démarrage du programme, SANS CODE MOTEUR   ║
 * ╚════════════════════════════════════════════════════════════════╝
 * 
 * ╔════════════════════════════════════════════════════════════════╗
 * ║  AVANTAGES :                                                   ║
 * ╠════════════════════════════════════════════════════════════════╣
 * ║  ✅ Ajouter un système = créer le .hpp, c'est TOUT            ║
 * ║  ✅ Pas besoin de modifier le moteur                          ║
 * ║  ✅ Pas de fichier RegisterSystems.cpp à maintenir            ║
 * ║  ✅ Découplage total moteur/systèmes de jeu                   ║
 * ║  ✅ Utilisable directement dans scenes.json                   ║
 * ╚════════════════════════════════════════════════════════════════╝
 */
#define AUTO_REGISTER_SYSTEM(SystemClass, SystemName) \
    private: \
    static inline bool _auto_registered_##SystemClass = []() { \
        Engine::SystemFactory::register_system<SystemClass>(SystemName); \
        return true; \
    }();

/**
 * @brief Version alternative si tu veux mettre la macro en public
 */
#define AUTO_REGISTER_SYSTEM_PUBLIC(SystemClass, SystemName) \
    public: \
    static inline bool _auto_registered_##SystemClass = []() { \
        Engine::SystemFactory::register_system<SystemClass>(SystemName); \
        return true; \
    }();
