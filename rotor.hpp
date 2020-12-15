#ifndef ROTOR_H
#define ROTOR_H

#include "errors.h"

using namespace std;

class Rotor {

public:
  
  /* Rotor information include: configuration, notches, starting position, current position */
  array<int, MAX_LETTERS> rotor;
  vector<int> notch;
  int starting_position;
  int current_position;
  
  /* Load Rotor from configuration file */
  Rotor(string const& file_name);

  /* Output Rotor information */
  void rotor_info();

  /* Rotate Rotor by one position */
  void rotor_rotate();

  /* Mirror Rotor configuration (Reflector to Plugboard) */
  int rotor_backwards(int letter_to_map);

};

#endif
