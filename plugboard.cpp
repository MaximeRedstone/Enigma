#include <iostream>
#include <cstring>
#include <array>
#include <fstream>

#include "errors.h"
#include "plugboard.hpp"

using namespace std;

/* Constructor for Plugboard Class */
Plugboard::Plugboard(string const& file_name) {

    // Create default mapping
    for (int i = 0; i < MAX_LETTERS; i++) {
      plugboard[i] = i;
    }
  
    ifstream in_stream;
    in_stream.open(file_name);
    
    int element_one, element_two;
    in_stream >> element_one >> element_two;
    
    // Swap two neighbouring elements to create mapping
    while (!in_stream.eof()) {
      plugboard[element_one] = element_two;
      plugboard[element_two] = element_one;
      in_stream >> element_one >> element_two;
    }
    in_stream.close();
}
