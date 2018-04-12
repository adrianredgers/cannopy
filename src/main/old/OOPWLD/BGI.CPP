// BGI.CPP : methods for Borland Graphics Interface "wrapper"
#include "bgi.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

bgi_graphics::bgi_graphics(char * path, graphics_drivers gdriv) {
  // initialize graphics and local variables:
  gdriver = gdriv;
  initgraph(&gdriver, &gmode, path);
  // read result of initialization:
  errorcode = graphresult();
  while (errorcode != grOk) {  /* an error occurred */
    // try again with local directory:
    initgraph(&gdriver, &gmode, ".");
    errorcode = graphresult();
    if(errorcode == grOk) break;
    printf("Graphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(1); /* terminate with an error code */
  }
  X_max = getmaxx();
  Y_max = getmaxy();
  maxcolor = (COLORS)getmaxcolor();
}

void bgi_graphics::bottomprompt(char * prompt) {
  const clipping = 1;  // clipping on
  setviewport(0, Y_max - textheight(prompt), X_max, Y_max, clipping);
  clearviewport();
  outtextxy(0,0, prompt);
  setviewport(0,0, X_max, Y_max, clipping);
}
