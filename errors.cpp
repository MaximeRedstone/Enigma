#include <fstream>
#include <iostream>
#include <cstring>
#include <array>
#include <vector>

#include "errors.h"

using namespace std;

const int NUMBER_OF_PAIRS = 13;
const int PLUGBOARD = 1;
const int REFLECTOR = 2;
const int ROTOR = 3;
const int MINIMUM_INPUT_FILES = 4;
const int MINIMUM_NUMBER = 0;
const int MAXIMUM_NUMBER = 25;

/* OUTPUT ERROR MESSAGES */
void ErrorMapping(int errorCode, string file) {
  switch(errorCode) {
  case 1: cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions"; break;
  case 2: cerr << " is not a valid input character (input characters must be upper case letters A-Z)!"; break;
  case 3: cerr << "INVALID_INDEX \n"; break;
  case 4: cerr << "Non-numeric character in file: " << file << endl; break;
  case 5: cerr << "IMPOSSIBLE_PLUGBOARD_CONFIGURATION \n" << endl; break;
  case 6: cerr << "Incorrect number of parameters in file: " << file << endl; break;
  case 7: cerr << "Not all inputs mapped in file: " << file << endl; break;
  case 8: cerr << "No starting position for rotor in file: " << file << endl; break;
  case 9: cerr << "Insufficient number of mappings in file: " << file << endl; break;
  case 10: cerr << "Incorrect number of parameters in file: " << file << endl; break;
  case 11: cerr << "Error opening configuration file: " << file << endl; break;
  default: cerr << "NO_ERROR" << endl; break;
  }
}

/* GENERAL ERROR HELPER FUNCTIONS */
int check_numeric_character(string number) {
  for (unsigned int count = 0; count < number.length(); count++) {
    if(!isdigit(number[count])) {
      return NON_NUMERIC_CHARACTER;
    }
  }
  return 0;
}

int check_invalid_index(string number) {
  if (stoi(number) < MINIMUM_NUMBER || stoi(number) > MAXIMUM_NUMBER) {
    return INVALID_INDEX;
  }
  return 0;
}

int check_pb_number_parameters(int count) {
  if(count > MAX_LETTERS) {
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  }
  return 0;
}

int check_pb_total_number_parameters(int count) {
  if(count % 2 == 1) {
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  }
  return 0;
}

/* PLUGBOARD ERROR HELPER FUNCTIONS */
int check_plugboard_config(int already_configured_numbers[26], string number, int count) {
  for (int i = 0; i < MAX_LETTERS; i++) {
    if (stoi(number) == already_configured_numbers[i]) {
      return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
    }
  }
  already_configured_numbers[count] = stoi(number);
  return 0;
}


/* REFLECTOR ERROR HELPER FUNCTIONS */
int check_reflector_config(int already_configured_numbers[26], string number_1, string number_2, int count) {
  
  if (count < NUMBER_OF_PAIRS) {
    for (int i = 0; i < MAX_LETTERS; i++) {
      if (stoi(number_1) == stoi(number_2)) {
	return INVALID_REFLECTOR_MAPPING;
      }
      if (stoi(number_1) == already_configured_numbers[i] && stoi(number_2) == already_configured_numbers[i]) {
	return INVALID_REFLECTOR_MAPPING;
      }
    }
    already_configured_numbers[count] = stoi(number_1);
    already_configured_numbers[count + 1] = stoi(number_1);
    }
  return 0;
}

int check_rf_number_of_pairs(int count) {
  if (count != NUMBER_OF_PAIRS) {
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }
  return 0;
}

   
/* ROTOR ERROR HELPER FUNCTIONS */
int check_rotor_config(int already_configured_numbers[26], string number_1, int count) {
  if (count != MAX_LETTERS) {
    for (int i = 0; i < MAX_LETTERS; i++) {
      if (stoi(number_1) == already_configured_numbers[i]) {
	return INVALID_ROTOR_MAPPING;
      }
    }
    already_configured_numbers[count] = stoi(number_1);
  }
  return 0;
}

int check_rot_all_mapped(int count) {
  if (count < MAX_LETTERS) {
    return INVALID_ROTOR_MAPPING;
  }
  return 0;
}

/* ROTOR POSITION ERROR HELPER FUNCTIONS */
int check_starting_position(int number_rotors, int number_positions) {
  if (number_rotors > number_positions) {
    return NO_ROTOR_STARTING_POSITION;
  }
  return 0;
}

/* FUNCTION LOADING ROTORS STARTING POSITIONS */
int load_starting_pos(vector<int>& starting_positions, string const& file_name, int number_rotors) {

  ifstream in_stream;
  in_stream.open(file_name);

  if (in_stream.fail()) {
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  int count = 0;
  string element;

  in_stream >> element;
  
  while(!in_stream.eof()) {
    count++;

    // Check valid positions
    if (check_numeric_character(element) != 0) {
      return check_numeric_character(element); }
    
    if (check_invalid_index(element) != 0) {
      return check_invalid_index(element); }

    int position = stoi(element);
    starting_positions.push_back(position);
    
    in_stream >> element;
  }

  // Check all rotors have a starting position
  if (check_starting_position(number_rotors, count) != 0) {
    return check_starting_position(number_rotors, count); }

  in_stream.close();
  return 0;
}

/* FUNCTION PERFORMING ALL CHECKS ON CONFIGURATION FILES */
int check_config_file(string const& file_name, int classname) {
  
  ifstream in_stream;
  in_stream.open(file_name);
  
  if (in_stream.fail()) {
    return 11;
  }

  // Plugboard file can be empty
  if (in_stream.eof() && classname == PLUGBOARD) {
    return 0;
  }
  
  string element;
  int count = 0;
  int rotor_counter = 0;
  int already_configured_numbers[26];
  int already_configured_notch[26];
  
  for (int i = 0; i < MAX_LETTERS; i++) {
    already_configured_numbers[i] = -1;
  }

  in_stream >> element;
  
  while(!in_stream.eof()) {
    count++;

    // Errors shared by all classes
    if (check_numeric_character(element) != 0) {
      return (check_numeric_character(element)); }
    
    if (check_invalid_index(element) != 0) {
      return check_invalid_index(element); }

    // Errors specific to plugboard
    if (classname == PLUGBOARD) {
      if (check_pb_number_parameters(count) != 0) {
	return (check_pb_number_parameters(count)); }
    
      if (check_plugboard_config(already_configured_numbers, element, count) != 0) {
	return check_plugboard_config(already_configured_numbers, element, count); }

      in_stream >> element;
    }

    //Errors specific to reflector
    else if (classname == REFLECTOR) {
      string element_1 = element;
      string element_2;

      if (count <= NUMBER_OF_PAIRS) {
	if (!(in_stream >> element).fail()) {
	  element_2 = element;
	}
	else {
	  return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
	}
	
	if (check_reflector_config(already_configured_numbers, element_1, element_2, count) != 0) {
	  return check_reflector_config(already_configured_numbers, element_1, element_2, count); }
      }
	in_stream >> element;
    } 
    // Errors specific to rotor
    else if (classname == ROTOR) {
      
      if (count <= MAX_LETTERS) {	
	rotor_counter++;
	if (check_rotor_config(already_configured_numbers, element, count) != 0) {
	  return (check_rotor_config(already_configured_numbers, element, count)); }
      }
      else {
	if (check_rotor_config(already_configured_notch, element, count) != 0) {
	  return (check_rotor_config(already_configured_notch, element, count)); }
      }
      in_stream >> element;
    }
    
  }

  // Check correct total number of parameters for each class
  if (classname == PLUGBOARD) {
    if (check_pb_total_number_parameters(count) != 0) {
      return (check_pb_total_number_parameters(count)); }
  }
  else if (classname == REFLECTOR) {
    if (check_rf_number_of_pairs(count) != 0) {
      return (check_rf_number_of_pairs(count)); }
  }
  else if (classname == ROTOR) {
    if (check_rot_all_mapped(rotor_counter) != 0) {
      return (check_rot_all_mapped(rotor_counter)); }
  }
  
  in_stream.close();
  return 0;
}
