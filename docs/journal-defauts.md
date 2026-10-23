# Journal des defauts - TP3
| ID | Etape | Exigence | Test revelateur | Symptome observe | Hypothese de cause | Technique |
|---|---|---|---|---|---|---|
| DEF-01 | 1 | EF-05 | test_EF05_range_check_bounds | Expected TRUE Was FALSE pour range_check(12500) | Borne superieure probablement exclue au lieu de comprise | Analyse des valeurs limites |
