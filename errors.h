/* error codes  */
#define INSUFFICIENT_NUMBER_OF_PARAMETERS         1
#define INVALID_INPUT_CHARACTER                   2
#define INVALID_INDEX                             3
#define NON_NUMERIC_CHARACTER                     4
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION        5
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS  6
#define INVALID_ROTOR_MAPPING                     7
#define NO_ROTOR_STARTING_POSITION                8
#define INVALID_REFLECTOR_MAPPING                 9
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS  10
#define ERROR_OPENING_CONFIGURATION_FILE          11
#define NO_ERROR                                  0
#define EMPTY_FILE                                12

#define MAX_LETTERS                               26

#include <iostream>
#include <cstring>
#include <array>
#include <vector>

using namespace std;

/* Function checks errors in configuration files */
void ErrorMapping(int errorCode, string file);

/* Function checks if input is digit */
int check_numeric_character(string number);

/* Function checks if index is valid (0-25) */
int check_invalid_index(string number);

/* Function checks if more than 26 connected letters */
int check_pb_number_parameters(int count);

/* Function checks if odd number of connected letters */
int check_pb_total_number_parameters(int count);

/* Function checks each number is mapped once */
int check_plugboard_config(int already_configured_numbers[MAX_LETTERS], string number, int count);

/* Function checks 13 pairs of connected letters for reflector */
int check_number_of_pairs(int count);

/* Function checks each letter mapped to different one and only mapped once */ 
int check_reflector_config(int already_configured_numbers[MAX_LETTERS], string number_1, string number_2, int count);

/* Function checks each letter mapped to different one and all are mapped */
int check_rotor_config(int already_configured_numbers[MAX_LETTERS], string number_1, int count);

/* Function checks all leters have been mapped */
int check_all_mapped(int count);

/* Function checks all rotors have starting position */
int check_starting_position(int number_rotors, int number_positions);
  
/* Function to load starting positions */
int load_starting_pos(vector<int>& starting_positions, string const& file_name, int number_rotors);

/* Functions performing all checks on configuration files */
int check_config_file(string const& file_name, int classname);
