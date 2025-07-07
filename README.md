# Distance_Interfaces_Methodes

Ce dépôt traite des méthodes Fast Marching (FMM), Fast Iterative (FIM), et Fast Sweeping (FSM) pour résoudre l'équation Eikonal. Il est structuré en trois dossiers principaux :

- **eikonal_d** : Contient le code pour les méthodes FMM, FIM, et FSM en 2D et 3D pour FIM et FSM.
- **eikonal_nouveau_maillage** : Contient le code pour les méthodes FIM et FSM en 2D avec des sources placées aléatoirement sur une maille plutôt qu'au centre, permettant d'expérimenter pour éviter les équidistances.
- **eikonal_parallele** : Contient le code parallèle pour les méthodes FIM et FSM en 2D et 3D.

## Prérequis

Pour exécuter les codes C++, les bibliothèques suivantes sont nécessaires :

- Standard Template Library pour les vecteurs et tuples.
- Les bibliothèques classiques d'écriture de fichiers en C++ : `os`, `fstream`.
- Numpy et `matplotlib.pyplot` pour la visualisation.
- OpenMP pour le calcul parallèle en C++.

## Exécution

Placez-vous dans l'un des trois dossiers et suivez les instructions ci-dessous :

### Eikonal_d

Pour compiler, utilisez la commande suivante : `g++ -o <executable> <main>`.

- **main2d.cxx** : Permet de tester les trois méthodes en 2D séquentiel et de générer un fichier texte avec la matrice des distances. Exécutez-le ainsi :
  ```sh
  ./<executable> <nombre_source> <nombre_point_sur_un_axe>
  ```

- **main3d.cxx** : Permet de tester les trois méthodes en 3D séquentiel et de générer un fichier VTK avec la matrice 3D des distances. Exécutez-le ainsi :
  ```sh
  ./<executable> <nombre_source> <nombre_point_sur_un_axe>
  ```

- **main2d_compare.cxx** : Permet de comparer les temps d'exécution sur une liste de nombres de points donnés pour un nombre de sources donné :
  ```sh
  ./<executable> <nombre_source> <nombre_de_nombre_de_points> <n_point1> [<n_point2> <...> <n_points_n>]
  ```

### Eikonal_nouveau_maillage

- **main2d.cxx** : Permet de tester les trois méthodes en 2D séquentiel et de générer un fichier texte avec la matrice des distances. Exécutez-le ainsi :
  ```sh
  ./<executable> <nombre_source> <nombre_point_sur_un_axe>
  ```

- **main2d_compare.cxx** : Permet de comparer les temps d'exécution sur une liste de nombres de points donnés pour un nombre de sources donné :
  ```sh
  ./<executable> <nombre_source> <nombre_de_nombre_de_points> <n_point1> [<n_point2> <...> <n_points_n>]
  ```

### Eikonal_parallele

- **main2d.cxx**, **main3d.cxx** : Permettent de tester les méthodes FSM et FIM en 2D et 3D en parallèle avec OpenMP.
  ```sh
  ./<executable> <nombre_source> <nombre_point_sur_un_axe> <n_threads>
  ```

- **main_comparaison.cxx** : Compare les temps d'exécution pour FSM, FIM et Euclidien par rapport au nombre de points et de sources en 2D parallèle.
  ```sh
  ./<executable> <num_n> <n1> <n2> ... <nk> <num_n_source> <n_source1> <n_source2> ... <n_sourcem>
  ```

- **main_comparaison_thread.cxx** : Compare les temps d'exécution de FSM, FIM et Euclidien par rapport au nombre de threads sur un maillage et un nombre de sources fixés.
  ```sh
  ./<executable> <n_source> <n> <n_threads1> [<n_threads2> ... <n_threadsk>]
  ```

## Visualisation

La visualisation se fait via matplotlib avec python.Dans chaque dossier il y a un sous-dossier **visu** avec plusieurs fichier python, permettant de visualiser les résultats obtenue, une visualisation de l'erreur si le fichier exact est fournis
et des visualisations de comparaison de performances sont aussi disponible.De plus il y a une fonction pour écrire des fichiers vtk pour faire de la visualisation **paraview** au besoin. 







