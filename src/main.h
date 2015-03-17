/*
    Program 1 Main Header File
    John "Jack" Riales, CSC230. J00515647
*/

#include <iostream>
#include <algorithm>
#include <cctype>
#include "binary.h"
#include "primary.h"
#include "secondary.h"
using namespace std;

// ===== Global constants
const std::string bin_filename = "binary_file.bin";
const std::string prime_index_filename = "primary_index.txt";
const std::string second_index_filename = "secondary_index.txt";
const std::string program_header = "University of South Alabama -- CS230 -- Program 1\nJohn \"Jack\" Riales, J00515647\n============\n\n";

// Running flag
bool running = true;

// Binary data array buffer
int buffer = PrimaryIndex::LISTING_BUFFER - 1;
// =====

// ===== Global main functions
bool binaryFileExists();                                        // Has the binary file already been created?
bool userPrompt_bulkBuild();                                    // Prompts the user to perform the bulk build (0) or to enter the menu program (1).
bool userPrompt_Confirmation(bool def, std::string message);    // Prompts the user for 'y' or 'n', and returns true or false respectively. Default will be the return for invalid input.
bool performBulkBuild(std::string input_filename);              // Gives the first prompt and allows the user to perform the bulk build if they want.
int  binaryMenu();                                              // Main switch that gives access to front end functions, such as adding and deleting
bool addRecord(BinaryData *obj);
bool changeRecord(BinaryData *obj);
bool deleteRecord(BinaryData *obj);
bool sellRecord (BinaryData *obj);
void write(BinaryData obj[], PrimaryIndex& prime_index, SecondaryIndex& second_index);
void quit();
void help();
void listData(BinaryData obj[]);
void print(BinaryData *obj, PrimaryIndex prime_index, SecondaryIndex second_index);
BinaryData recordPrompt();
BinaryData getByTitle(BinaryData *obj);
BinaryData getByType(BinaryData *obj);
std::string toLowerCase(std::string str);
// =====

// ======= Used for the help display
const int possible_inputs = 9;

const char input_list[] = {
	'a',    // Add record
	'c',    // Change record
	'd',    // Delete record
	's',    // Sell a record
	'w',    // Write record
	'q',    // Quit menu
	'h',    // Display help
	'l',    // List data
	'p',    // Print
};

const std::string help_display_strings[] = {
	"Add a record to the database.",
	"Change an existing record.",
	"Delete a record from the database.",
	"Sell a record",
	"Commit and write the changes to the binary file and the indexes.",
	"Quit the program.",
	"Display this help message.",
	"List all valid data",
	"Search for and print an entry."
};
// =======
