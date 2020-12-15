#include <iostream>
#include <cstring>
#include <array>
#include <fstream>

#include "errors.h"
#include "reflector.hpp"

using namespace std;

/* Constructor for Reflector Class */
Reflector::Reflector(string const& file_name) {
    
    ifstream in_stream;
    in_stream.open(file_name);
    
    int element_one, element_two;
    in_stream >> element_one >> element_two;
    
    // Swap two neighbouring elements to create mapping
    while (!in_stream.eof()) {
      reflector[element_one] = element_two;
      reflector[element_two] = element_one;  
      in_stream >> element_one >> element_two;
    }
    in_stream.close();
}
