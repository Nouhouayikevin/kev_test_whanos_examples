# Test du Toggle de la Force

## Instructions

1. Lance le jeu :
```bash
./YourExecutableName 2>&1 | grep -E "Force|🔵|⚫|🔑"
```

2. Pendant que le jeu tourne :
   - Appuie sur **G** plusieurs fois
   - Note ce qui s'affiche dans la console

## Ce qui DEVRAIT s'afficher

```
🔵 ForceInitSystem: Checking for Forces and Player...
🔵 Force #5 linked to player #4
✅ ForceInitSystem: 1 Force(s) initialized

[Premier appui sur G après 500ms]
🔑 G key pressed! Current state: ACTIVE
⚫ Force: DEACTIVATED (toggled to false)

[Deuxième appui sur G]
🔑 G key pressed! Current state: INACTIVE
🔵 Force: ACTIVATED (toggled to true)

[Troisième appui sur G]
🔑 G key pressed! Current state: ACTIVE
⚫ Force: DEACTIVATED (toggled to false)
```

## Si ça ne marche pas

Si tu ne vois QUE le premier message de désactivation et JAMAIS le message de réactivation :
- Le problème est que `isKeyPressed(KeyCode::G)` ne détecte plus les appuis après le premier
- Possibilité : la fenêtre perd le focus ou il y a un problème avec l'InputManager

## Solution alternative

Si le problème persiste, on peut utiliser `isKeyDown` au lieu de `isKeyPressed` avec un debounce manuel.
