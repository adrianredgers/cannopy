// DIRECTST2.CPP : test of derived class dirfile2
#include "direct2.hpp"

main() {
  const size = 20;
  // you can't use a const this way with ANSI C!:
  dirfile2 dirlist[size]; // Only difference from before!
  ffblk file_info;
  int i = 0;
  // find all different types of files:
  int done = findfirst("*.*", &file_info, 0x37);
  while(!done) {
    dirlist[i++].setfile(&file_info);  // member function call
    if(i >= size) {
      puts("out of dirlist space");
      break;  // out of while loop
    }
    done = findnext(&file_info);
  }
  i = 0;
  while(dirlist[i++].print())
    ;  // do it until the function returns 0 (end of the list)
  for(i = 0; i < size; i++)
    dirlist[i].print2();
}
