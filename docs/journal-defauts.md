# Journal des defauts - TP3
| ID | Etape | Exigence | Test revelateur | Symptome observe | Hypothese de cause | Technique |
|---|---|---|---|---|---|---|
| DEF-01 | 1 | EF-05 | test_EF05_range_check_bounds | Expected TRUE Was FALSE pour range_check(12500) | Borne superieure probablement exclue au lieu de comprise | Analyse des valeurs limites |
| DEF-02 | 2 | EF-03 | test_EF03_transient_partial_window et test_EF03_invalid_sample_is_excluded ; SAN: test_EF03_steady_state_five_identical | Expected 100 Was 2600 ; Expected 2500 Was 2650 ; lecture hors limites filter.c:30 | Indice 5 utilise pour window[5], probablement une borne de boucle incorrecte | Partitions de sequences et sanitizers |
