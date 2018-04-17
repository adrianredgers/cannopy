// DIRECT.HPP : objects to store file directory information
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <stdio.h>

class dirfile {
protected:  // means derived classes have access
  char filename[14];
  char filepath[MAXPATH];
  long size;
  char attribute;
  int time, date;
public:
  dirfile() { // constructor
    size = 0L;
    attribute = 0;
    time = date = 0;
    filename[0] = '\0';  // null-terminated string
    filepath[0] = '\0';
  }
  // No destructor (cleanup) needed here.
  void setfile(ffblk * file_info) {
    strcpy(filename, file_info->ff_name);
    size = file_info->ff_fsize;
    attribute = file_info->ff_attrib;
    time = file_info->ff_ftime;
    date = file_info->ff_fdate;
    getcwd(filepath, MAXPATH);
  }
  int print() {
    if(attribute & FA_DIREC) 
      return printf("subdirectory: %s\\%s\n", filepath, filename);
    if(size)
      return printf("%s\\%s  %ld bytes\n", filepath, filename, size);
    return 0;
  }
  int fsize() { return size; }  // access function
  // a second way to print a file:
  void print2() {
    if(!*filename) return;  // don't print empty objects
    if(attribute & FA_DIREC) { // print directories with a '\'
      char buf[14];
      strcpy(buf, filename);
      strcat(buf, "\\");
      printf("%-15s", buf);
      return;
    }
    printf("%-15s", filename);
  }
};
