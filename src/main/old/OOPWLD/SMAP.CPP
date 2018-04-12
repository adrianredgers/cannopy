// SMAP.CPP
#include "smap.hpp"
#include <stdio.h>
#include <conio.h>  // borland
#include <stdlib.h>
#define cprintf printf
#define cputs puts

screenmap::screenmap(int ht, int wid) : width(wid), height(ht) {
  map = new char*[height];
  for(int i = 0; i < height; i++) {
    map[i] = new char[width + 1];
    for(int j = 0; j < width; j++)
      map[i][j] = ' ';
    map[i][j] = 0;  // null terminator
  }
}

screenmap::~screenmap() {
  for(int i = 0; i < height; i++)
    delete map[i];
  delete map;
}

char & screenmap::operator() (int h, int w) {
  static char dummy;
  if(w < 0 || w >= width) {
    fprintf(stderr,"operator() width out of range");
    return dummy;
  }
  if(h < 0 || h >= height) {
    fprintf(stderr,"operator() height out of range");
    return dummy;
  }
  return map[h][w];
}

void screenmap::display() { 
  for(int i = 0; i < height; i++) 
    cputs(map[i]);
}
