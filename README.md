# Projet Quoridor

- [Sujet du projet](https://www.labri.fr/perso/renault/working/teaching/projets/2021-22-S6-C-Flood.php)

- [Page sur thor](https://thor.enseirb-matmeca.fr/ruby/projects/projetss6-flood)

# Authors

 - Mouhcine Ahbaiz <mahbaiz@enseirb-matmeca.fr>
 - Hicham Nekt <hnekt@enseirb-matmeca.fr>
 - Atman Boz <aboz@enseirb-matmeca.fr>
 - Mohamed Yassine Kabouri <mkabouri@enseirb-matmeca.fr>



# Coding convention
***

Naming
------
* Functions: lower case with underscore between words
* Variable: first letter with upper case
* Macro: full upper case with an underscore between the words
* Objects: full lower case

braces
------
* openning accolade: to be put in the current line
* closing accolade: to be put alone in the next line

Compilation
-----------
* `make` : compilation of the whole project
    - Execution: `./server -m $1 -t $2 -f $3 -a $4 -S $5 ./libplayer1.so ./libplayer2.so` 
    or libplayer3.so instead of libplayer1.so or libplayer2.so because we have three players.  
        where: 
               - $1: width matrix graph. (obligatory).
               - $2: graph shape. values {'c', 'd', 'h', 't'}.  (optional: 'c' as default shape).
               - $3: number of forbidden colors. values {0, 1, ..., 8}. (optional: 0 as default value).
               - $4: graph coloration mode. values {0, 1, 2, 3}. (optional: 1 as default value, random coloration).
               - $5: display mode. values {0, 1}. (optional: 0 as default and 1 for sdl display).
                

* `make install` : compilation and generation of a dynamic libraries in install/ directory.

* `make test` : compilation of tests
    - Execution of tests: ./alltests
    
