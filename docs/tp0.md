# TP0 — Installation et prise en main de ThermoNode

Tests firmware : 2 tests, 0 echec.
Tests Python : 1 passed. Analyse cppcheck : 0 constat. Pylint : 9,01/10.
Couverture des lignes : 2,5 %. Flash : 10148/49152 octets. RAM : 1640/8192 octets. Trois services Docker operationnels. /health : ok. Trames MQTT observees, rejetees pour CRC invalide.
Q1 - hal.h definit une interface entre le programme et le materiel. Ses implementations permettent de travailler avec le materiel reel, un simulateur ou des doublures de test.
Q2 - Non. Les deux tests fournis couvrent peu de comportements. Leur reussite ne prouve pas une absence de defauts.
Q3 - La passerelle intercepte les erreurs de trame et compte les rejets sans planter. Cela contribue a EF-G2, referencee dans C-03. Notre trame malformee a ete rejetee au controle CRC : cet essai seul ne valide pas le traitement syntaxique JSON.
Q4 - ADC:4095 injecte le code brut 4095. La conversion donne 27991 centi-degres, soit 279,91 degres Celsius. Cette valeur depasse la plage physique autorisee : le noeud doit passer en FAULT selon EF-05.
