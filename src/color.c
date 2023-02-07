/**
 * \file color.c
 *
 *
 */
#include "color.h"
#include "graph.h"


const char* color_to_string(enum color_t c)
{
    char *ColorString[9] = {"NO_COLOR", "BLUE", "RED", "GREEN", "YELLOW", "ORANGE", "VIOLET", "CYAN", "PINK"};
    return ColorString[c+1];
}


void colorate_hole (enum color_t matrix_colors[], int  size, int hole_diameter, char shape) {
  if (shape == 'c' || shape == 't') return;
  int center_donut = (size/2)*size+(size/2);
  int centers_h[2]={center_donut-hole_diameter*size,center_donut+hole_diameter*size};
  int d=hole_diameter/2;
  switch (shape) {
    case 'd':
      for (int i=-d; i<=d;i++){
        for (int j=-d; j<=d;j++){
          matrix_colors[center_donut+i*size+j]=NO_COLOR;//BLACK
        }
      }
      break;
    case 'h':
      for (int i=-d; i<=d;i++){
        for (int j=-d; j<=d;j++){
          matrix_colors[centers_h[0]+i*size+j]=NO_COLOR;//BLACK
          matrix_colors[centers_h[1]+i*size+j]=NO_COLOR;//BLACK
        }
      }
      break;
    default:
      printf("please indicate the shape of the graph");
      abort();
  } 
}

void colorate_random ( enum color_t matrix_colors[], enum color_t colors[MAX_COLOR], size_t size, size_t hole_diameter, char shape) {
  for (size_t i=0 ; i < size*size ;i++) {
    matrix_colors[i]=colors[rand()%MAX_COLOR];
  }
  if (matrix_colors[0] == matrix_colors[size*size-1]){
    matrix_colors[size*size-1]=5;
  }
  colorate_hole (matrix_colors,  size,  hole_diameter,  shape);
}

void colorate_cyclic ( enum color_t matrix_colors[], enum color_t colors[MAX_COLOR], size_t size, size_t hole_diameter, char shape) {
  matrix_colors[0]=colors[rand()%MAX_COLOR];
  for (size_t i=1 ; i < size*size ;i++) {
    matrix_colors[i]=(matrix_colors[i-1]+1)%MAX_COLOR;
  }
  if(matrix_colors[0] == matrix_colors[size*size-1]){
    matrix_colors[size*size-1] = (matrix_colors[0]+5)%MAX_COLOR;
  }
  colorate_hole (matrix_colors,  size,  hole_diameter,  shape);
}

void colorate_bloc ( enum color_t matrix_colors[], enum color_t colors[MAX_COLOR], size_t size, size_t hole_diameter, char shape) {
  matrix_colors[0]=colors[rand()%MAX_COLOR];
  for (size_t i=1 ; i < size*size/2 ;i++) {
    matrix_colors[i]=(matrix_colors[i-1]+1)%MAX_COLOR;
  }
  matrix_colors[size*size/2]=colors[rand()%MAX_COLOR];
  for (size_t i=(size*size)/2+1 ; i < size*size ;i++) {
    matrix_colors[i]=(matrix_colors[i-1]+1)%MAX_COLOR;
  }
  for (size_t i=0 ; i < 6*size ;i++) {
    matrix_colors[rand()%(size*size)]=colors[rand()%MAX_COLOR];
  }
  if(matrix_colors[0] == matrix_colors[size*size-1]){
    matrix_colors[size*size-1] = (matrix_colors[0]+5)%MAX_COLOR;
  }
  colorate_hole (matrix_colors,  size,  hole_diameter,  shape);
}


void colorate_symetric (enum color_t COL[], enum color_t colors[MAX_COLOR], size_t size, size_t hole_diameter, char shape){
  for (size_t i=0 ; i < size*size/2 ;i++) {
    int col=rand()%MAX_COLOR;
    COL[i]=colors[col];
    COL[size*size-i-1]=colors[col];
  }
  if (COL[0] == COL[size*size-1]){
    COL[size*size-1]=5;
  }
  colorate_hole (COL,  size,  hole_diameter,  shape);
}

/**
 * This function chooses the game coloration mode.
 */

void choose_colorate(int colorate,enum color_t matrix_colors[], enum color_t colors[MAX_COLOR], size_t size, size_t hole_diameter, char shape){
  switch (colorate)
    {
    case 0:
      colorate_cyclic ( matrix_colors, colors, size,hole_diameter,shape);
      break;
    case 1:
      colorate_random ( matrix_colors, colors, size,hole_diameter,shape);
      break;
    case 2:
      colorate_bloc ( matrix_colors, colors, size,hole_diameter,shape);
      break;
    case 3:
      colorate_symetric ( matrix_colors, colors, size,hole_diameter,shape);
      break;
    default:
      printf("\n this coloration is not valide chose between 0 1 or 2\n");
      abort();
      break;
    } 
}
