/*
    Program 1 Main Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include <iostream>
#include "binary.h"
#include "primary.h"
#include "secondary.h"
using namespace std;

// Global constants
const std::string bin_filename = "binary_file.bin";
const std::string prime_index_filename = "primary_index.txt";
const std::string second_index_filename = "secondary_index.txt";

// Global main functions
bool binaryFileExists();                                // Has the binary file already been created?
bool userPrompt_bulkBuild();                            // Prompts the user to perform the bulk build (0) or to enter the menu program (1).
bool performBulkBuild(std::string input_filename);      // Gives the first prompt and allows the user to perform the bulk build if they want.
int  binaryMenu();                                      // Main switch that gives access to front end functions, such as adding and deleting

// ========================================================= // MAIN

int main (int argc, char** argv)
{
    // Start by asking if the user wants to rebuild the binary file, or go to the menu
    if (!userPrompt_bulkBuild()) {
        // Ask what the input filename is
        std::string input_filename;
        cout << "Please enter the input file name: ";
        cin  >> input_filename;

        // Build the file and the indexes.
        if (!performBulkBuild(input_filename))
            return 0;

        // Ask to go to menu
        char goto_menu;
        cout << "Go to menu? (y/n): ";
        cin  >> goto_menu;
        switch (goto_menu) {
        case 'y':
            return binaryMenu();
        case 'Y':
            return binaryMenu();
        default:
            cout << "Exiting.";
            return 0;
        }
    }
    else {
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
    }
    else {
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
    switch (result) {
    case 0:
        char confirmation;
        cout << "Are you sure you wish to rebuild the binary file? (y/n): ";
        cin  >> confirmation;
        switch (confirmation) {
        case 'y':
            return 0;
        case 'Y':
            return 0;
        case 'n':
            cout << "Confirmation returned 'n'. Entering menu program.\n";
            return 1;
        case 'N':
            cout << "Confirmation returned 'n'. Entering menu program.\n";
            return 1;
        default:
            cout << "Confirmation input invalid. Entering menu program.\n";
        }
    case 1:
        cout << "Entering menu program.\n";
        return 1;
    default:
        cout << "Unknown case. Entering default input (1).\n";
        return 1;
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
    }
    else {
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
    }
    else {
        cout << "Index streams could not be opened. Exiting.";
        return false;
    }
}

// ========================================================= //

int binaryMenu()
{
    return 0;
}
