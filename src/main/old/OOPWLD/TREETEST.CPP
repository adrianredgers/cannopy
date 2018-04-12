// TREETEST.CPP : test of directory tree (sorted directory)
#include "dirtree.hpp"
#include <stdlib.h>

main(int argc, char * argv[]) {
  char * filespec = (argc > 1 ? argv[1] : "*.*");
  ffblk file_info;
  int done = findfirst(filespec, &file_info, 0x37);
  if(done) exit(0);  // no files to find!
  dirtree tree(new dirfile3(&file_info));
  int count = 0;
  while(!done) {
    tree.add(new dirfile3(&file_info)); // sloppy, but OK
    count++;
    done = findnext(&file_info);
  }
  dlist dl(count); // make a list big enough to hold all elements
  tree.distribute(dl); // place the elements sequentially in dl
  puts("\n---------- Horizontal listing ------------");
  for(int x = 0; x < count; x++)
    dl[x]->print2();
  puts("\n---------- Vertical Columns --------------");
  const rows = count/4 + 1;
  // an array of 4 dlists:
  dlist column[] = { rows, rows, rows, rows };
  int ndx = -1, i, j;
  for(j = 0; j < 4; j++)
    for(i = 0; i < rows; i++) {
      if(++ndx >= count) break;
      column[j].add(dl[ndx]); // distribute among the four columns
    }
  dirfile3::reset_printcount();  // call a static member function
  for(i = 0; i < rows; i++)
    for(j = 0; j < 4; j++)
      column[j][i]->print2(); // print the columns out vertically
}
