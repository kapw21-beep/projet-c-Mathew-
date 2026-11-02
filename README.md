# Editeur de formes SVG en C

Projet en langage C utilisant principalement `struct`, `int`, `char`, `if`, `else`, `switch`, `case`, `for`, `while`, `do`, `void` et `return`.

## Fichiers

- `main.c` : programme complet.
- `Makefile` : compilation et lancement.
- `README.md` : explications du projet.

## Compilation et lancement

Dans un terminal, depuis le dossier du projet :

```sh
make
make run
```

Ou sans Makefile :

```sh
cc -std=c11 main.c -o formes
./formes
```

## Fonctions disponibles

1. Créer un cercle
2. Créer une ellipse
3. Créer un carré
4. Créer un rectangle
5. Créer une ligne
6. Créer une multiligne
7. Créer un polygone
8. Créer une trajectoire
9. Créer un groupe de formes
10. Afficher l'inventaire des formes et groupes
11. Modifier une forme
12. Supprimer une forme
13. Voir le détail d'un groupe
14. Supprimer un groupe
15. Exporter le dessin SVG et l'ouvrir sur macOS
16. Sauvegarder le projet
17. Importer un projet sauvegardé
0. Quitter

## Export et sauvegarde

L'option 15 crée `dessin.svg` (800 x 600). Ce fichier s'ouvre dans un navigateur. Sur macOS, le programme tente de l'ouvrir automatiquement. Les formes sont dessinées avec des couleurs simples nommées, sans code RGB ni dégradés.

L'option 16 enregistre les formes et les groupes dans `projet_formes.txt`. L'option 17 recharge ce fichier. Pour conserver le travail, utiliser la sauvegarde avant de quitter.

## Simplification

Les animations du terminal, les codes RGB, le mineur animé, l'aperçu Braille et la bibliothèque externe Olive.c ont été retirés. La visualisation graphique est assurée par le fichier SVG ouvert dans le navigateur, sans bibliothèque graphique supplémentaire. Les types de formes et les options du menu d'origine sont conservés.
