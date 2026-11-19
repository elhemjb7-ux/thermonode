# Dérogations MISRA C:2012 — ThermoNode

| ID | Règle | Fichier | Justification | Approbation |
|---|---|---|---|---|
| DEV-001 | 11.4 (conversion pointeur ↔ entier) | hal/hal_stm32.c (via STM32Cube) | Accès aux registres mappés en mémoire par la bibliothèque HAL du fabricant | Responsable qualité |
| DEV-002 | 21.6 (`<stdio.h>`) | src/frame.c, src/node.c | `snprintf` utilisé pour la sérialisation JSON avec contrôle de la longueur ; alternative (sérialisation manuelle) jugée plus risquée | Responsable qualité |
| DEV-003 | 21.6 (`<stdio.h>`) | hal/hal_sim.c | Code de simulation hôte, non embarqué | — |
| DEV-004 | 15.5 (point de sortie unique) | Fonctions de `firmware/src/` signalées par Cppcheck | Les retours anticipés servent de gardes. Ils rejettent les cas invalides avant le traitement principal et limitent l’imbrication. | À demander en revue |
| DEV-005 | 18.4 (arithmétique de pointeurs) | `firmware/src/frame.c` | Les offsets servent à repérer les champs JSON, mesurer leurs longueurs et isoler le champ CRC. La dérogation est limitée à ce module. | À demander en revue |
| DEV-006 | 21.1 et 21.10 (macro réservée et `<time.h>`) | `firmware/src/main.c`, blocs `TN_SIM` | Le simulateur hôte utilise `_POSIX_C_SOURCE` et `nanosleep`. Ces blocs sont exclus du build embarqué. | À demander en revue |
