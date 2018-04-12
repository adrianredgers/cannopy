// BGI.HPP : Class to "wrap" around Borland Graphics Interface
#ifndef _BGI_HPP_
#define _BGI_HPP_
#include <graphics.h>

class bgi_graphics {
  static int gdriver;
  static int gmode;
  static int errorcode;
  static int X_max;  // maximum graphics screen coordinates
  static int Y_max;
public:
  static COLORS maxcolor;
  // Constructor default: request auto detection
  bgi_graphics(char * path = "", graphics_drivers gdriv = DETECT);
  ~bgi_graphics() { closegraph(); }  /* clean up */
  static int xmax() { return X_max; }
  static int ymax() { return Y_max; }
  // Text output (in graphics mode) on the bottom line:
  static void bottomprompt(char * prompt);
};
#endif // _BGI_HPP_
