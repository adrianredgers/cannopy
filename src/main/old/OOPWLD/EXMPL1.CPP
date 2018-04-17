// C++ comments go to the end of the line
/* C-style comments still work */

class typename {
  int internal_data;  // private
public: // accessible to the outside world
  typename() { // constructor has same name as class
    internal_data = 0;  // perform initialization in constructor
  }
  ~typename() { // destructor is class name plus a tilde
    // perform cleanup in destructor
  }
  void external_operation() { internal_data++; }
  int access_function() { return internal_data;}
  // The above are "inline" functions.  
  // Here's a non-inline declaration:
  void set(int x);
};  // end of class must have a semicolon!

// here's the non-inline definition:
void typename::set(int x) {
  internal_data = x;
}

// here's how we use the class: 
main() {
  typename X;  // just like defining a variable of a built-in type
  X.external_operation();  // just like accessing a struct member
  int a = X.access_function();
  X.set(100);
}
