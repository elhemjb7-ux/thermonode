# Revue de MR !7 « fix hysteresis »

Réviseurs : <ELHEM JABALLI>
Commit revu : `39adf61` (patch adapté au fichier de tests de notre dépôt).

| # | Fichier et ligne | Constat | Règle ou source | Gravité | Demande à l’auteur |
|---|---|---|---|---|---|
| 1 | `firmware/src/threshold.c:31` | La constante `500` ignore l’hystérésis configurée. | EF-04, constantes | Bloquant | Rétablir le calcul avec `hyst_centi`. |
| 2 | `firmware/src/node.c:22` | Un identifiant d’au moins `TN_ID_LEN` caractères peut remplir le tampon sans terminateur `\0`. Ce changement dépasse le périmètre de la MR. | Revue C, chaînes et tampons | Bloquant | Retirer ce changement de la MR et conserver la terminaison explicite. |
| 3 | `firmware/test/test_threshold.c:59,91` | Le test de l’hystérésis configurable est renommé et son appel commenté. La suite passe avec 8 tests au lieu de 9. | Revue des tests | Majeur | Réactiver le test et vérifier qu’il passe avec la correction. |
| 4 | `firmware/src/threshold.c:31–38` | Précédence à expliciter, accolades manquantes et opérateur virgule. | MISRA 12.1 et 15.6 signalées par l’outil, 12.3 relevée à la lecture | Mineur | Parenthéser la soustraction, remettre les accolades et séparer les deux affectations. |
| 5 | Message du commit `39adf61` | `fix hysteresis` ne donne ni portée ni référence à EF-04. | Traçabilité Git | Mineur | Utiliser un message explicite qui cite EF-04. |

## Rapports de défaut

### DR-TP4-01 — Hystérésis configurée ignorée

- Commit et environnement : `39adf61`, VM Ubuntu, GCC 13.
- Reproduction : compiler un programme qui appelle `threshold_set(6000, 300)`, puis `threshold_step(5699, true)`, avec `firmware/src/threshold.c`.
- Attendu : `THR_CLEAR = 2`, car 56,99 °C est sous la limite de 57 °C.
- Obtenu : `THR_NONE = 0`. La MR utilise une limite de 55 °C.
- Sévérité : majeure. L’alerte peut rester active après le franchissement de la limite configurée.
- Priorité : à corriger avant fusion. Référence : EF-04.
- Preuve : sortie du test ciblé, `Attendu THR_CLEAR = 2` et `Obtenu = 0`.

### DR-TP4-02 — Identifiant potentiellement non terminé

- Commit et environnement : `39adf61`, VM Ubuntu, GCC 13.
- Reproduction par inspection : suivre `node_set_id` avec une chaîne de `TN_ID_LEN` caractères avant son premier `\0`.
- Attendu : le tampon conserve un `\0` final, quitte à tronquer l’identifiant.
- Obtenu par inspection : `strncpy(node_id, id, TN_ID_LEN)` remplit le tampon et aucune instruction ne force ensuite le `\0`.
- Sévérité : majeure. Une opération qui attend une chaîne C peut lire au-delà du tampon.
- Priorité : à corriger avant fusion. Référence : liste de contrôle C, chaînes et tampons.
- Preuve : `git diff main...39adf61 -- firmware/src/node.c`.

## Q1 — Complémentarité des techniques

1. La lecture révèle la constante `500`. Le test ciblé confirme la régression. La suite de la MR la laisse passer car le test correspondant est désactivé.
2. La lecture révèle le risque de terminaison. Cppcheck classique ne l’a pas signalé sur les fichiers analysés.
3. Le diff montre la désactivation du test. La sortie d’exécution confirme la baisse de 9 à 8 tests dans la suite du seuil.
4. L’addon MISRA signale 12.1 à la ligne 31 et 15.6 aux lignes 34, 36 et 38. La lecture relève aussi l’opérateur virgule. Une suite verte ne vérifie pas ces règles de style.
5. La lecture du message de commit révèle le manque de traçabilité. Ni les tests ni l’analyse du C ne vérifient ce message.

Cppcheck classique 2.13.0 a émis uniquement des messages informatifs sur les deux fichiers analysés. La lecture, l’analyse statique et le test ciblé ont apporté des preuves différentes.

## Décision — Demander des changements

Rétablir l’hystérésis configurable et réactiver le test associé.
Retirer le changement de terminaison de l’identifiant du périmètre de la MR.
Corriger les écarts de style et référencer EF-04 dans le message de commit.
