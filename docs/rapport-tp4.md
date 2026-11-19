# Rapport qualité TP4 — ThermoNode

**Étudiante** — Elhem Jaballi

## 1. Revue de la MR !7 — Q1

La revue du commit `39adf61` est consignée dans `docs/review-MR7.md`.
Décision : demander des changements. La branche de revue est restée hors de `main`.

Le test ciblé a confirmé que la MR ignore l’hystérésis configurée. Avec un
seuil de 60 °C, une hystérésis de 3 °C et une mesure de 56,99 °C, on attend
`THR_CLEAR` (2). La MR renvoie `THR_NONE` (0). Le test qui aurait révélé
cette régression avait été désactivé. La lecture du diff a aussi relevé
un risque de chaîne non terminée dans `node_set_id`. Cppcheck classique
n’a émis que des messages informatifs. L’addon MISRA a relevé les écarts
de précédence et d’accolades. La lecture, l’analyse et le test ciblé ont
chacun apporté une preuve différente.

## 2. Analyse statique — Q2

Le détail par règle, les classifications et les commits sont dans
`docs/analyse-statique-tp4.md`. Sur la version locale réunissant les quatre PR,
les signalements de l’addon MISRA passent de **64 à 0** après corrections
et suppressions justifiées. Les dérogations proposées sont documentées
dans `docs/misra-deviations.md` et restent à valider en revue.

Pylint passe de **9,01 à 9,79/10**. Le test unitaire Python disponible passe.
Les exceptions locales, comme celle du nom `do_GET` imposé par `http.server`,
laissent les règles actives ailleurs. Le contrôle des doublons désactivé
reste visible dans les résultats pylint et nécessite un test dédié.

## 3. SonarQube — Q3

| Indicateur | Avant (`tp3-baseline`) | Après intégration sur `main` |
|---|---:|---|
| Couverture globale SonarQube | 24,4 % | À relever |
| Duplications | 0,0 % | À relever |
| Alertes ouvertes en sécurité, fiabilité et maintenabilité | 0 dans chaque catégorie | À relever |
| Security Hotspots | 0 | À relever |
| Note de maintenabilité | A | À relever |
| Quality Gate TVD | Passed | À relever |

Le verdict de référence porte sur le nouveau code. La couverture du code
existant reste de 24,4 %. Le rapport Cppcheck classique a donné zéro constat
et SonarQube a importé zéro issue externe lors de cette analyse.

SonarQube importe la couverture Python depuis `reports/coverage-gw.xml`.
La couverture du firmware est mesurée séparément par `make -C firmware coverage`.
Le rapport de référence gcovr donne **89,7 % des lignes** et **82,6 % des branches**.
La qualité du firmware reste donc à vérifier avec ses rapports et ses tests,
en complément de la gate SonarQube.

## 4. Gestion de configuration — Q4 et Q5

| PR | Objet | État |
|---|---|---|
| #1 | Corrections MISRA du firmware | En attente de revue |
| #2 | Corrections pylint de la passerelle | En attente de revue |
| #3 | Dérogations et suppressions MISRA | En attente de revue |
| #4 | Hystérésis par défaut à 4 °C et commentaire EF-04 | En attente de revue |

Le conflit a été reproduit localement par un rebase de la branche de B sur
celle de A. La résolution a gardé `400` (4 °C) et le commentaire de B adapté.
Les **50 tests firmware** ont passé après le rebase. La première publication
de B a utilisé un push normal. Sur une branche déjà publiée, `--force-with-lease`
permet de refuser la réécriture si la branche distante a changé depuis
la version attendue. Un test manque pour exercer la valeur par défaut
de bout en bout sans la remplacer pendant l’initialisation.

Le tag cible est `v0.2.0`, un jalon pendant le développement initial.
Une correction rétrocompatible seule après ce jalon donnerait `0.2.1`.
Un changement incompatible du format de trame doit être documenté dans
l’ICD et signalé par la politique de versionnage. Après stabilisation
de l’interface publique en `1.x`, une rupture implique une nouvelle majeure.

## 5. Plan de réduction de la dette

1. Ajouter un test Python qui expose le défaut de gestion des doublons, puis corriger le traitement.
2. Ajouter un test firmware qui exerce l’hystérésis par défaut à 4 °C.
3. Étendre les tests Python au-delà du seul test unitaire présent pour améliorer la couverture.

## 6. Références

- [Dépôt et PR](https://github.com/elhemjb7-ux/thermonode/pulls)
- [Projet SonarQube ThermoNode](http://localhost:9000/summary/overall?id=thermonode)
- Référence Git de l’analyse initiale : `c95d536`. Revue de la MR suspecte : `39adf61`.
- Bilan d’analyse statique : `2fdf928`. Tag final : à compléter après intégration.
