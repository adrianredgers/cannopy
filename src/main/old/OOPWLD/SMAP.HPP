// SMAP.HPP : class for mapping the character screen, so you can place
// characters in the map, and display the map.

class screenmap {
  char ** map;
  int width;
  int height;
public:
  screenmap(int ht, int wid);
  ~screenmap();
  char & operator() (int h, int w);
  void display();
};
