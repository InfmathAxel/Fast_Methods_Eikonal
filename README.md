# Distance_Interfaces_Methodes

Repository à propos des méthodes Fast Marching,Fast Iteratives et Fast sweeping method pour résoudre l'équation Eikonal. Avec 3 dossiers principaux :
- eikonal_d :  Contient le code pour les méthodes FMM,FIM,FSM  en 2D et 3D pour la FIM et FSM 
- eikonal_nouveau_maillage :Contient le code pour les méthodes FIM,FSM en 2D avec des sources placer aléatoire sur une mailles plutot que centrés, permet d'expérimenter pour couper les équidistances.
- eikonal_parallele : Contient le code parallèle pour les méthodes FIM et FSM 2D et 3D.


## Requierement 

Pour executer les codes c++ on a besoin des librairies suivante :
- Standard templat library pour les vecteurs, tuples
- Les librairies classique d'écriture de fichiers c++ , os,fstream 
- Numpy et matplotlib.pyplot pour la visualisation
- OpenMP pour le calcul parallèle c++

## Execution 

Se placer dans l'un des 3 dossiers et pour éxécuter :

### Eikonal_d

pour compiler on effectuera la commande  ```g++ -o <executable> <main> ``` 

- main2d.cxx permet de tester les 3 méthodes en 2d séquentielle et d'avoir un fichier txt avec la matrice des distances. on l'éxécute ainsi : 
```sh
./<executable> <nombre source> <nombre point sur un axe>
```


- main3d.cxx permet de tester les les 3 méthodes en 3d séquentielle et d'avoir un fichier vtk avec la matrice  3D des distances. on l'éxécute ainsi :
```sh
./<executable> <nombre source> <nombre point sur un axe>
```

- main2d_compare.cxx permet de comparer les temps d'execution sur une liste de nombre de points données sur un nombre de sources données 
```sh
./<executable>  <nombre source > <nombre de nombre de points > <n_point1> [<n_points2> < ...> <n_points n>]
```


### Eikonal_nouveau_maillage

- main2d.cxx permet de tester les 3 méthodes en 2d séquentielle et d'avoir un fichier txt avec la matrice des distances. on l'éxécute ainsi : 
```sh
./<executable> <nombre source> <nombre point sur un axe>
```

- main2d_compare.cxx permet de comparer les temps d'execution sur une liste de nombre de points données sur un nombre de sources données 
```sh
./<executable>  <nombre source > <nombre de nombre de points > <n_point1> [<n_points2> < ...> <n_points n>]
```


### Eikonal_parallele

- main2d.cxx , main3d.cxx permet de tester les méthodes FSM, FIM 2d et 3d en parallèle openmp.

```sh
./<executable> <nombre source> <nombre point sur un axe> <n_threads>
```

-main_comparaison.cxx compare les temps d'executions pour la FSM,FIM et euclidien par rapport au nombre de points et de sources en 2D parallèle.

```sh
./<executable> <num_n> <n1> <n2> ... <nk> <num_n_source> <n_source1> <n_source2> ... <n_sourcem>
```

-main_comparaison.cxx compares les temps d'executions de la FSM,FIm et euclidien par rapport au nombre de threads sur un maillage et nombre de source fixé. 
```sh
./<executable><n_source> <n> <n_threads1> [<n_threads2> ... <n_threadsk>]
```



## Visualisation








