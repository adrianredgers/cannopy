// SHED.CPP: graphic shape editor
// #define EXTEND  // un-comment to add a new feature to the system
#include "polygon.hpp"
#include "ellipse.hpp"
#if defined(EXTEND)
#include "circbox.hpp"  // An "extension" to the system
#endif
#include "doskey.hpp"
#include "shlist.hpp"
// Note the above contain standard C library includes.

// BGI initialization:
char * bgipath = "d:\\t\\g\\"; // path with BGI files
bgi_graphics BGI(bgipath);  // can also link directly; see manual

shape * makeshape() {
  const x = bgi_graphics::xmax()/2, y = bgi_graphics::ymax()/2;
  while(1) {
    bgi_graphics::bottomprompt(
      "c: circle, e: ellipse, r: rectangle, s: square, t: triangle"
#if defined(EXTEND)
      ", b: circlebox"  // must also extend the menu
#endif
    );
    switch(get_doskey()) {
      case CAP_C: case LC_C: return new Circle(x, y, 10);
      case CAP_E: case LC_E: return new Ellipse(x, y, 10, 30);
      case CAP_R: case LC_R: return new Rectangle(x,y);
      case CAP_S: case LC_S: return new Square(x, y);
      case CAP_T: case LC_T: return new Triangle(x, y);
      // Must also extend the switch statement:
#if defined(EXTEND)
      case CAP_B: case LC_B: return new circlebox(x,y, 10);
#endif
      default: break;
    }
  }
}

main() {
  shape_list shapelist;
  // make some initial shapes:
  int verts1[] = { 0, 0, 0, 20, 50, 50, 75, 0 };
  shapelist.add(new Polygon(sizeof(verts1)/(sizeof(verts1[0]) * 2),
            verts1, 100,300, 20,20, WHITE, HATCH_FILL));
  shapelist.add(new Ellipse(200,200, 50, 75, WHITE, HATCH_FILL));
  shapelist.add(new Ellipse(100,200, 70, 75, WHITE, HATCH_FILL));
  shapelist.add(new Square(300,300, 100));
  shapelist.add(new Circle(350,350, 47));

  shape * sh = shapelist.top();
  shapelist.drawlist();
  while(1) {
    bgi_graphics::bottomprompt(
      "a: add shape, r: remove, n: next shape, "
      "arrow keys: move, e: edit, esc to quit");
    const move_factor = 20;
    switch(get_doskey()) {
      case CAP_A       : // Add a shape
      case LC_A        : sh = shapelist.add(makeshape());
                         sh->draw();
                         break;
      case CAP_R       :
      case LC_R        : shapelist.remove();
                         sh = shapelist.top();
                         shapelist.drawlist();
                         break;
      case CAP_N       : // move to next valid shape in the list
      case LC_N        : sh = shapelist.next();
                         if(!sh) sh = shapelist.top();
                         if(!sh) return 0;  // empty list
                         sh->draw();
                         break;
      case LEFT_ARROW  : sh->moverelative(- move_factor, 0);
                         shapelist.drawlist();
                         break;
      case RIGHT_ARROW : sh->moverelative(move_factor, 0);
                         shapelist.drawlist();
                         break;
      case UP_ARROW    : sh->moverelative(0, - move_factor);
                         shapelist.drawlist();
                         break;
      case DOWN_ARROW  : sh->moverelative(0,  move_factor);
                         shapelist.drawlist();
                         break;
      case CAP_E       :
      case LC_E        : sh->edit();
                         shapelist.drawlist();
                         break;
      case ESCAPE      : return 0;
      default          : break;
    }
  }
}
