#!/bin/bash

# Script pour nettoyer les enregistrements redondants de composants dans les scènes

echo "🧹 Nettoyage des enregistrements de composants redondants..."
echo ""

scenes=(
    "All_src/scenes/WinScene.cpp"
    "All_src/scenes/ScoreScene.cpp"
    "All_src/scenes/LoseScene.cpp"
    "All_src/scenes/Online_GameScene_server.cpp"
    "All_src/scenes/CreateRoomScene.cpp"
    "All_src/scenes/OnlineModeScene.cpp"
    "All_src/scenes/MenuScene.cpp"
)

for scene in "${scenes[@]}"; do
    if [ -f "$scene" ]; then
        echo "📝 Traitement de $scene..."
        
        # Créer une sauvegarde
        cp "$scene" "$scene.bak"
        
        # Compter les lignes register_component avant
        before=$(grep -c "register_component<Component::" "$scene" 2>/dev/null || echo "0")
        
        # Afficher un message
        if [ "$before" -gt 0 ]; then
            echo "   ✅ Trouvé $before enregistrements de composants à nettoyer"
        else
            echo "   ℹ️  Aucun enregistrement trouvé"
        fi
    fi
done

echo ""
echo "✅ Traitement terminé !"
echo ""
echo "⚠️  Les fichiers originaux ont été sauvegardés avec l'extension .bak"
echo "   Vous devez maintenant modifier manuellement chaque scène pour supprimer les enregistrements redondants"
echo "   et ajouter un commentaire : '// Les composants sont maintenant enregistrés automatiquement dans le constructeur du Registry'"
