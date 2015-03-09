/*
    Program 1 Main Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include <iostream>
#include <cctype>
#include "binary.h"
#include "primary.h"
#include "secondary.h"
using namespace std;

// Global constants
const std::string bin_filename = "binary_file.bin";
const std::string prime_index_filename = "primary_index.txt";
const std::string second_index_filename = "secondary_index.txt";
const std::string program_header = "University of South Alabama -- CS230 -- Program 1\nJohn \"Jack\" Riales, J00515647\n============\n\n";

// Global main functions
bool binaryFileExists();                                // Has the binary file already been created?
bool userPrompt_bulkBuild();                            // Prompts the user to perform the bulk build (0) or to enter the menu program (1).
bool userPrompt_Confirmation(bool def, std::string message); // Prompts the user for 'y' or 'n', and returns true or false respectively. Default will be the return for invalid input.
bool performBulkBuild(std::string input_filename);      // Gives the first prompt and allows the user to perform the bulk build if they want.
int  binaryMenu();                                      // Main switch that gives access to front end functions, such as adding and deleting

// ========================================================= //

int main (int argc, char** argv)
{
	cout << program_header;

	// Start by asking if the user wants to rebuild the binary file, or go to the menu
	if (!userPrompt_bulkBuild()) {
		// Ask what the input filename is
		std::string input_filename;
		cout << "Please enter the input file name: ";
		cin  >> input_filename;

		// Build the file and the indexes.
		if (!performBulkBuild(input_filename))
			return 0;

		if (userPrompt_Confirmation(false, "Go to menu?"))
			return binaryMenu();
		else {
			cout << "Exiting.";
			return 0;
		}
	} else {
		if (binaryFileExists())
			return binaryMenu();
		else
			cout << "Binary file nonexistant. Exiting.";
	}
	return 0;
}

// ========================================================= //

bool binaryFileExists()
{
	// Do a quick open of the file we're looking for (binary_file.bin, in this case) and check if it's good
	ifstream in(bin_filename.c_str());
	if (in.good()) {
		in.close();
		return true;
	} else {
		in.close();
		return false;
	}
}

// ========================================================= //

bool userPrompt_bulkBuild()
{
	int result;
	cout << "Please enter (0) to perform the bulk build, or enter (1) to enter the menu program: ";
	cin  >> result;

	if (cin.fail()) {
        cout << "Unknown case. Entering default input (1).\n";
		return 1;
	}

	switch (result) {
	case 0:
        if (userPrompt_Confirmation(false, "Are you sure you wish to rebuild the binary file?")) {
            return 0;
        }
        else return 1;
	case 1:
		cout << "Entering menu program.\n";
		return 1;
	default:
		cout << "Unknown case. Entering default input (1).\n";
		return 1;
	}
}

// ========================================================= //

bool userPrompt_Confirmation(bool def, std::string message)
{
    char confirmation;
    cout << message << " (y/n): ";
    cin  >> confirmation;

    // Make it lower case
    if (isupper(confirmation))
        confirmation = char(tolower(confirmation));

    switch (confirmation) {
    case 'y':
        return true;
    case 'n':
        return false;
    default:
        return def;
    }
}

// ========================================================= //

bool performBulkBuild(std::string input_filename)
{
	// Create an array of data objects as large as the buffer located in the PrimaryIndex class
	unsigned int buffer = PrimaryIndex::LISTING_BUFFER - 1;
	BinaryData obj [buffer];

	// Create the indexes
	PrimaryIndex prime_index;
	SecondaryIndex second_index;

	// Open the stream to the input file
	std::fstream in (input_filename.c_str(), ios::in);
	if (in.good()) {
		cout << "Reading input file.\n";

		// Read each line onto each object
		for (unsigned int i = 0; i < buffer; i++) {
			obj[i].read_sequential(in, i);
		}
	} else {
		cout << "Input file nonexistant. Exiting bulk build.\n";
		return false;
	}

	// Close the input stream
	in.close();

	// Create the output stream
	std::fstream out (bin_filename.c_str(), ios::out | ios::binary);
	if (out.good()) {
		cout << "Writing binary file.\n";

		// Write out the binary file
		for (unsigned int i = 0; i < buffer; i++) {
			obj[i].write (out, i);
		}
	}

	// Close output
	out.close();

	// Read in the indexes from the object array
	cout << "Reading objects for index creation.\n";
	prime_index.read(obj, buffer);
	second_index.read(obj, buffer);

	// Open output streams to write out the files
	std::fstream    prime_out (prime_index_filename.c_str(), ios::out);
	std::fstream    second_out (second_index_filename.c_str(), ios::out);

	if (prime_out.good() && second_out.good()) {
		cout << "Writing indexes.\n";

		// Write the indexes
		prime_index.write(prime_out);
		second_index.write(second_out);

		// Close the streams
		prime_out.close();
		second_out.close();

		return true;
	} else {
		cout << "Index streams could not be opened. Exiting.";
		return false;
	}
}

// ========================================================= //

// == Used for the help display
const int possible_inputs = 7;

const char input_list[] = {
	'a',    // Add record
	'c',    // Change record
	'd',    // Delete record
	'w',    // Write record
	'q',    // Quit menu
	'h',    // Display help
	'l'     // List data
};

const std::string help_display_strings[] = {
	"Add a record to the database.",
	"Change an existing record.",
	"Delete a record from the database.",
	"Commit and write the changes to the binary file and the indexes.",
	"Quit the program.",
	"Display this help message.",
	"List all valid data"
};
// ==

// Menu functions
bool addRecord(BinaryData *obj);
bool changeRecord(BinaryData *obj);
bool deleteRecord(BinaryData *obj);
void write(BinaryData obj[], PrimaryIndex prime_index, SecondaryIndex second_index);
void quit();
void help();
void listData(BinaryData obj[]);

// These are used by the menu functions
BinaryData recordPrompt();

// Running flag
bool running = true;

// Binary data array buffer
int buffer = PrimaryIndex::LISTING_BUFFER - 1;

int binaryMenu()
{
    // Reset cin, in case the doofus at the keyboard put in a char for the bulk build/menu input.
    cin.clear();

    // Read back in the binary file
    fstream in (bin_filename, ios::in | ios::binary);
    BinaryData *objects = new BinaryData[buffer];
    for (int i = 0; i < buffer; i++)
        objects[i].read_binary(in, i);

    // Resize the global buffer to be only valid entries that came from the input file
    for (int i = 0; i < buffer; i++) {
        if (objects[i].title() == "0")
            buffer--;
    }

    // Regenerate the indexes
    PrimaryIndex    prime_index;
    SecondaryIndex  second_index;
    prime_index.read(objects, buffer);
    second_index.read(objects, buffer);

    // Start menu
	char input;
	while (running) {
		cout << "\nEnter a command or type 'h' for a list of commands.\n> ";
		cin  >> input;

		// If input is upper case, make sure it's lower case
		if (isupper(input))
			input = char(tolower(input));

		switch (input) {
		case 'h':
			help();
			break;
		case 'q':
			quit();
			break;
		case 'w':
			write(objects, prime_index, second_index);
			break;
		case 'd':
			deleteRecord(objects);
			break;
		case 'c':
			changeRecord(objects);
			break;
		case 'a':
			addRecord(objects);
			break;
        case 'l':
            listData(objects);
            break;
        default:
            cout << "Invalid command. Displaying help.\n\n";
            help();
		}
	}

	return 0;
}

bool addRecord(BinaryData *obj)
{
    // Check for jumps in the array (deleted items with title == "0")
    for (int i = 0; i < buffer; i++) {
        // Put the add there if it exists
        if (obj[i].title() == "0") {
            obj[i] = recordPrompt();
            return true;
        }
    }

    // Increment the buffer
    buffer += 1;

    // Buffer exceeds index range exception
    if (buffer >= PrimaryIndex::LISTING_BUFFER) {
        cout << "Buffer exceeds Primary Index listing buffer. Cannot add.\n";
        buffer -= 1;
        return false;
    }

    // Buffer can increase, copy over data to a new array
    else {
        // Create new array of a new buffer size and copy over
        BinaryData *_new = new BinaryData[buffer];
        for (int i = 0; i < buffer - 1; i++)
            _new[i] = obj[i];

        // Prompt for a new entry at the end of new array
        _new[buffer-1] = recordPrompt();

        // Point over to the new array
        delete [] obj;
        obj = _new;
    }

    return true;
}

bool changeRecord(BinaryData *obj)
{
    int input;
    cout << "Choose a record to formulate changes to (or -1 to cancel).\n";

    // Iterate and display all valid entries (Title != "0")
    listData(obj);

    // Take input
    cout << "Input> ";
    cin  >> input;

    // Cancel
    if (input <= -1) {
        cout << "Cancelled. Returning.\n";
        return true;
    }

    // Object exceeds buffer
    else if (input >= buffer) {
        cout << "Input exceeds buffer. Returning.\n";
        return false;
    }

    // Valid input
    else {
        // Take object input and emplace it onto that record
        obj[input] = recordPrompt();
    }

    return true;
}

bool deleteRecord(BinaryData *obj)
{
    int input;
    cout << "Choose a record to delete (or -1 to cancel).\n";

    // Iterate and display all valid entries (Title != "0")
    listData(obj);

    // Take input
    cout << "Input> ";
    cin  >> input;

    // Cancel
    if (input <= -1) {
        cout << "Canceled. Returning.\n";
        return true;
    }

    // Object exceeds buffer
    else if (input >= buffer) {
        cout << "Input exceeds buffer. Returning.\n";
        return false;
    }

    // Valid entry
    else {
        if (!obj[input].is_deleted())
            obj[input] = *(new BinaryData);
        else
            cout << "Object already flagged for deletion.\n";
    }
    return true;
}

void write(BinaryData obj[], PrimaryIndex prime_index, SecondaryIndex second_index)
{
    fstream bin_out (bin_filename, ios::out | ios::binary);
    fstream prime_out (prime_index_filename, ios::out);
    fstream second_out (second_index_filename, ios::out);

	if (userPrompt_Confirmation(false, "Are you sure you wish to commit and write the changes?")) {
	    // Write changes binary
        for (int i = 0; i < buffer; i++)
            obj[i].write(bin_out, i);

        // Let the indexes read the binary
        prime_index.read(obj, buffer);
        second_index.read(obj,buffer);

        // Write them out
        prime_index.write(prime_out);
        second_index.write(second_out);
        cout << "Changes committed and written to file system.\n";
	}
	else
		cout << "Changes will not be committed. Returning.\n";

	// Close and finish
	bin_out.close();
    prime_out.close();
    second_out.close();
}

void quit()
{
	cout << "Quitting menu program...\n";
	running = false;
}

void help()
{
	for (int i = 0; i < possible_inputs; i++)
		cout << input_list[i] << "\t" << help_display_strings[i] << endl;
}

void listData(BinaryData obj[])
{
    for (int i = 0; i < buffer; i++) {
        if (obj[i].title() != "0")
            cout << i << ") " << obj[i].to_string() << endl;
    }
}

BinaryData recordPrompt()
{
    std::string title, artist, type;
    int year, price, count;

    cout << "Enter a title: ";
    cin  >> title;
    cout << "Enter an artist: ";
    cin  >> artist;
    cout << "Enter a type: ";
    cin  >> type;
    cout << "Enter a year: ";
    cin  >> year;
    cout << "Enter a price: ";
    cin  >> price;
    cout << "Enter count: ";
    cin  >> count;

    BinaryData result(title, artist, type, year, price, count);
    return result;
}
