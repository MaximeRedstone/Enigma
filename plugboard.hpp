#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include "errors.h"

using namespace std;

class Plugboard {
  
public:
  
  /* Plugboard information include: mapping */
  array<int, MAX_LETTERS> plugboard;

  /* Load Plugboard from configuration file */
  Plugboard(string const& file_name);
};

#endif
