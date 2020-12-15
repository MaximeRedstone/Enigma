#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "errors.h"

using namespace std;

class Reflector {
  
public:

  /* Reflector information include: mapping */
  array<int, MAX_LETTERS> reflector;

  /* Load Reflector from configuration file */
  Reflector(string const& file_name);
};

#endif
