#include <iostream>
#include <istream>
#include <array>
#include <vector>
#include <cstring>
#include <fstream>
#include <cctype>

#include "errors.h"
#include "plugboard.hpp"
#include "reflector.hpp"
#include "rotor.hpp"

using namespace std;

const int MINIMUM_INPUT_FILES = 4;
const int PLUGBOARD_REF = 1;
const int REFLECTOR_REF = 2;
const int ROTOR_REF = 3;
const int AT_LEAST_ONE_ROTOR = 3;

int main(int argc, char** argv) {
  
  int errorCode = 0;
  string empty_str = " ";

  // Check input parameters
  if (argc < MINIMUM_INPUT_FILES) {
    errorCode = 1;
    ErrorMapping(errorCode, empty_str);
    exit(errorCode);
  }
  
  /* LOADING CONFIGURATION FILES FOR EACH COMPONENT OF ENIGMA */
  
  /* PLUGBOARD */
  string plugboard_file = argv[1];
  errorCode = check_config_file(argv[1], PLUGBOARD_REF);
  if (errorCode != 0) {
    ErrorMapping(errorCode, plugboard_file);
    exit(errorCode);
  }
  Plugboard plugboard = Plugboard(argv[1]);
  
  /* REFLECTOR */
  string reflector_file = argv[2];
  errorCode = check_config_file(argv[2], REFLECTOR_REF);
  if (errorCode != 0) {
    ErrorMapping(errorCode, reflector_file);
    exit(errorCode);
  }
  Reflector reflector = Reflector(argv[2]);

  /* ROTOR */
  vector<Rotor> all_rotors;
  vector<int> starting_positions;

  // If rotors have been specified, create them
  int number_parameters = argc - 1;
  if (number_parameters > AT_LEAST_ONE_ROTOR) {
    for (int i = AT_LEAST_ONE_ROTOR; i < number_parameters; i++) {
      string rotor_file = argv[i];
      errorCode = check_config_file(argv[i], 3);
      
      if (errorCode != 0) {
	ErrorMapping(errorCode, rotor_file);
	exit(errorCode);
      }
      
      Rotor rotor_i = Rotor(argv[i]);
      all_rotors.push_back(rotor_i);
    }
  }

  int number_of_rotors = all_rotors.size();
  
  /* ROTOR STARTING POSITION */
  errorCode = load_starting_pos(starting_positions, argv[number_parameters], number_of_rotors);
  if (errorCode != 0) {
    ErrorMapping(errorCode, argv[number_parameters]);
    exit(errorCode);
  }
  
  if (number_of_rotors > 0) {
    // Assign each rotor its starting position
    for (int unsigned i = 0; i < starting_positions.size(); i++) {
      all_rotors[i].starting_position = starting_positions[i];
      all_rotors[i].current_position = starting_positions[i];
    }
    
    // Rotate each Rotor to be aligned with their starting position
    for (int i = number_of_rotors - 1; i >= 0; i--) {
      int current_starting_position = all_rotors[i].starting_position;
      int rotation_number = 0;
      
      if (current_starting_position != 0) {
	while (rotation_number < current_starting_position) {
      	  all_rotors[i].rotor_rotate();
	  rotation_number++;
	}
      }
    }
  }
  
  /* STARTING ENCODING / DECODING INPUT */
  
  string sentence;
  getline(cin, sentence);
  
  for(int unsigned l = 0; l <sentence.length(); l++) {

    char letter = sentence.at(l);

    // Check for valid input
    if (!isspace(static_cast<int>(letter))) {
      if (isdigit(letter) || islower(letter) != 0) {
	errorCode = 2;
	cerr << letter;
	ErrorMapping(errorCode, empty_str);
	exit(errorCode);
      }

      int letter_idx = letter - 65;
      
      // Map letter on plugboard
      int letter_to_rotors = plugboard.plugboard[letter_idx];

      // Updating each rotor state
      int letter_to_reflector;
      int past_letter = letter_to_rotors;
      
      if (number_of_rotors > 0) {
	// Rotate rotor everytime a key is pressed
	all_rotors[number_of_rotors - 1].rotor_rotate();

	// Update absolute frame of reference (12 o'clock)
	all_rotors[number_of_rotors - 1].current_position += 1;
	
	int abs_frame = all_rotors[number_of_rotors - 1].current_position;

	// Look at all rotors
	for (int i = number_of_rotors - 1; i >= 0; i--) {
	  // Check if it hits a notch
	  for (int unsigned j = 0; j < all_rotors[i].notch.size(); j++) {
	    if ((abs_frame % MAX_LETTERS) == all_rotors[i].notch[j]) {
	      if (i >= 1){
		// Rotate rotor to its left
		all_rotors[i - 1].rotor_rotate();
		all_rotors[i - 1].current_position += 1;
	      }
	    }
	  }
	}

	// Map letter through rotors (forwards)
	for (int i = number_of_rotors - 1; i >= 0; i--) {
	  letter_to_reflector = all_rotors[i].rotor[past_letter];
	  past_letter = letter_to_reflector;
	}
      }
      
      else {
	letter_to_reflector = letter_to_rotors;
      }

      // Map letter on reflector
      int letter_from_reflector = reflector.reflector[letter_to_reflector];

      // Map returned letter on rotors (backwards)
      int letter_to_plugboard;
      int next_letter = letter_from_reflector;
      if (number_of_rotors > 0) {
	for (int i = 0; i < number_of_rotors; i++) {
	  letter_to_plugboard = all_rotors[i].rotor_backwards(next_letter);
	  next_letter = letter_to_plugboard;
	}
      }
      else {
	letter_to_plugboard = letter_from_reflector;
      }

      // Map returned letter on plugboard
      int letter_out = plugboard.plugboard[letter_to_plugboard];
      
      char output_char = letter_out + 65;
      cout << output_char;
    }
  }
  
  cout << endl;
  errorCode = 0;
  ErrorMapping(errorCode, empty_str);
  return errorCode;
}
