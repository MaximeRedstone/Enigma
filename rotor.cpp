#include <iostream>
#include <cstring>
#include <array>
#include <fstream>
#include <vector>

#include "errors.h"
#include "rotor.hpp"

using namespace std;

/* Constructor for Rotor Class */
Rotor::Rotor(string const& file_name) {
  
    ifstream in_stream;
    in_stream.open(file_name);

    int count = 0;
    int element_one;
    in_stream >> element_one;
    
    while (!in_stream.eof()) {

      // Load rotor configuration
      if (count < MAX_LETTERS) {
	rotor[count] = element_one;
	in_stream >> element_one;
      }
      
      // Load notch positions
      else if (count >= 26) {
        int notch_number = 0;
	notch.push_back(element_one);
	notch_number++;
	in_stream >> element_one;
      }
      count++;
    }
    in_stream.close();
}

/* Rotate Rotor by one position */
void Rotor::rotor_rotate() {

  int difference = 0;
  vector<int> all_differences;
  int rotor_content;

  // Find current mapping for all letters
  for (int idx = 0; idx < 26; idx++) {
    rotor_content = this->rotor[idx];
    difference = rotor_content - idx;
    all_differences.push_back(difference);
  }

  // Shift the current mapping by 1
  int temp = all_differences[0], k;
  for (k = 0; k < MAX_LETTERS; k++) {
    all_differences[k] = all_differences[k + 1];
    
  }
  all_differences[k-1] = temp;

  // Create new rotor mapping
  for (int idx = 0; idx < 26; idx++) {
    if (all_differences[idx] + idx >= 0) {
      // % 26 to make sure output always 0 - 25
      this->rotor[idx] = (all_differences[idx] + idx) % 26;
    }
    else if (all_differences[idx] + idx < 0) {
      // + 26 to make sure output always 0 - 25
      this->rotor[idx] = (all_differences[idx] + idx) + 26;
    }
  }
}

/* Mirror Rotor configuration (Reflector to Plugboard) */
int Rotor::rotor_backwards(int letter_to_map) {

  int letter_mapped;
  int rotor_lhs;
  
  for (int idx = 0; idx < 26; idx++) {
    rotor_lhs = this->rotor[idx];
    if (letter_to_map == rotor_lhs) {
      letter_mapped = idx;
      return letter_mapped;
    }
  }
  return 0;
}

/* Output one Rotor's information (configuration, notches, starting positions) */
void Rotor::rotor_info() {

    cout << "The rotor information includes: \n";
    for (int k = 0; k < 26; k++) {
      cout << this->rotor[k] << " ";
    }

    cout << "\nThis rotor contains a notch at position: \n";
    for (int unsigned k = 0; k < this->notch.size(); k++) {
      cout << this->notch[k] << " ";
    }
    
    cout << "\nThis rotor has starting position: " << this->starting_position << endl;
}
