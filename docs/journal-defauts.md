# Journal des defauts - TP3
| ID | Etape | Exigence | Test revelateur | Symptome observe | Hypothese de cause | Technique |
|---|---|---|---|---|---|---|
| DEF-01 | 1 | EF-05 | test_EF05_range_check_bounds | Expected TRUE Was FALSE pour range_check(12500) | Borne superieure probablement exclue au lieu de comprise | Analyse des valeurs limites |
| DEF-02 | 2 | EF-03 | test_EF03_transient_partial_window et test_EF03_invalid_sample_is_excluded ; SAN: test_EF03_steady_state_five_identical | Expected 100 Was 2600 ; Expected 2500 Was 2650 ; lecture hors limites filter.c:30 | Indice 5 utilise pour window[5], probablement une borne de boucle incorrecte | Partitions de sequences et sanitizers |
| DEF-03 | 4 | EF-04 | test_EF04_counter_resets_below_threshold | Expected 0 Was 1 : alerte apres 70,70,25,70,70 degres | Compteur probablement non remis a zero lors du retour sous le seuil | Table de decision et sequence interrompue |
| DEF-06 | 5 | EF-05 | test_EF05_recovers_to_normal_after_5_valid_samples | Expected 1 Was 3 : reste en FAULT apres cinq mesures valides | Transition FAULT vers NORMAL absente ou desactivee dans le noeud | Tests de transitions avec capteur et horloge simules |
