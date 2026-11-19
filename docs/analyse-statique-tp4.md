# Analyse statique ThermoNode — TP4

Mesure avant sur `main` (`c95d536`). Mesure après sur la branche locale
`verification/tp4`, qui réunit les changements des quatre PR.
Outils : Cppcheck 2.13.0 avec l’addon MISRA C:2012 et pylint 3.3.1.

## Cppcheck et MISRA

Cppcheck classique sur la version de référence : 0 constat.
Addon MISRA : 64 signalements avant, 0 après corrections et suppressions justifiées.

| Règle | Avant | Après | Classe | Action et commit |
|---|---:|---:|---|---|
| 15.5 | 25 | 0 | Dérogation proposée | Retours anticipés dans les gardes, DEV-004 (`faea816`). |
| 17.7 | 9 | 0 | Violation de norme | Vérifier les retours de `snprintf` et ignorer explicitement les autres retours (`100d98f`). |
| 5.9 | 4 | 0 | Violation de norme | Distinguer les noms internes entre le filtre et le tampon circulaire (`0b3fda4`). |
| 18.4 | 4 | 0 | Dérogation proposée | Arithmétique de pointeurs limitée au module de trames, DEV-005 (`faea816`). |
| 10.8 | 4 | 0 | Violation de norme | Séparer les conversions des calculs et comparer le temps en non signé (`e84c5fd`). |
| 12.3 | 3 | 0 | Signalement de style | Séparer les déclarations signalées par l’addon (`bb0b68b`). |
| 12.1 | 3 | 0 | Violation de norme | Expliciter la précédence avec des parenthèses (`d9447f0`). |
| 8.9 | 2 | 0 | Violation de norme | Réduire la portée de deux variables statiques (`6f928b9`). |
| 8.7 | 2 | 0 | Violation de norme et faux positif | Rendre `state_name` interne (`8229709`). Justifier l’appel à `range_check` depuis les tests (`faea816`). |
| 21.6 | 2 | 0 | Dérogation documentée | Suppression ciblée pour l’usage de `stdio.h` dans `frame.c` et `node.c`, DEV-002 (`faea816`). |
| 7.4 | 1 | 0 | Faux positif | `node_id` est un tableau de `char` initialisé par un littéral. Suppression locale (`faea816`). |
| 2.5 | 1 | 0 | Violation de norme et faux positif intermédiaire | Utiliser les macros de période EF-08 (`e1b3314`). Justifier localement `_POSIX_C_SOURCE` pour le simulateur (`7977eab`). |
| 22.8 | 1 | 0 | Violation de norme | Initialiser `errno` et vérifier les conversions du seuil et du CRC (`3340c32`, `9b2fa52`, `9294145`). |
| 21.10 | 1 | 0 | Dérogation proposée | `time.h` pour le simulateur hôte, DEV-006 (`faea816`). |
| 21.1 | 1 | 0 | Dérogation proposée | Macro POSIX pour le simulateur hôte, DEV-006 (`faea816`). |
| 13.3 | 1 | 0 | Violation de norme | Séparer l’affectation du numéro de séquence et son incrément (`5870df9`). |
| 22.10 | 0 | 0 | Signalement intermédiaire | Simplifier l’appel à `strtoul` et contrôler `errno` juste après (`9294145`). |

## Pylint — Q2

Le score passe de **9,01/10 (19 messages)** à **9,79/10 (4 messages)** avec
le commit `5699227`. Le test unitaire Python disponible dans le dépôt passe.

- Conventions corrigées : docstrings de classe (`C0115`), `maxsplit` (`C0207`)
  et une instruction par ligne (`C0321`).
- Contraintes d’API : `do_GET` (`C0103`) est le nom attendu par `http.server`.
  L’exception est locale et commentée. Les arguments inutilisés des callbacks
  Paho (`W0613`) restent à leur place et sont préfixés par `_`.
- Conception à examiner : `R0913` et `R0917` dans `Storage` signalent une
  signature longue. Les deux messages restent visibles.
- Défaut fonctionnel à traiter séparément : `W0511` et `R1727` pointent vers
  le même contrôle des doublons désactivé dans `handler.py`.

Une exception locale limite la suppression au cas justifié et laisse la règle
active pour le reste du projet. Côté MISRA, les faux positifs sont traités par
des commentaires ciblés, et les dérogations par des entrées dans
`docs/misra-deviations.md` et les suppressions correspondantes.
