/*
    Secondary Index Class Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "secondary.h"
using namespace std;

// Constructor
SecondaryIndex::SecondaryIndex(int count):
	_count (count)
{
#ifdef _DEBUG_
	printf("Instantiating a new secondary index with count value %d\n", count);
#endif

	// Iterate and instantiate with default values
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		_listing[i]._type = "0";
		_listing[i]._duplicates = 0;
	}
}

SecondaryIndex::~SecondaryIndex()
{
	; // Nothing to see here... yet!
}

bool SecondaryIndex::set_type_at(unsigned int key, std::string type)
{
	// Key exceeds buffer exception
	if (key >= LISTING_BUFFER) {
#ifdef _DEBUG_
		printf("Error! Given key exceeds index buffer range of primary index.\n");
#endif

		return false;
	} else {
		// If type does not already exist, add it
		if (!type_match(type)) {
#ifdef _DEBUG_
			printf("Setting type at given key %d to %s\n", key, type.c_str());
#endif

			_listing[key]._type = type;
			return true;
		}
	}
	return false;
}

void SecondaryIndex::update_type(std::string type)
{
	// None of this type found, make a new entry at the next null spot
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		if (_listing[i]._type == type) {
#ifdef _DEBUG_
			printf ("Duplicate exists at %d. Incrementing duplication tag for %s.\n", i, type.c_str());
#endif // _DEBUG_

			_listing[i]._duplicates++;
		} else if (_listing[i]._type == "0") {
			set_type_at(i, type);
		}
	}
}

bool SecondaryIndex::write (std::fstream& out)
{
	// Error check stream
	if (!out) {
#ifdef _DEBUG_
		printf ("Output stream is not open. Please check your parameters.\n");
#endif // _DEBUG_

		return false;
	}

#ifdef _DEBUG_
	printf ("Writing Secondary Index to output stream.\n");
#endif // _DEBUG_

	// Header
	out << "Listing #\tType\t\tDuplicates\t";

	// Iterate through listing
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		// Output; one line per listing
		out << "\n"                             // New line
		    << i << "\t\t"                      // Output the position of the list we're at
		    << _listing[i]._type << "\t\t"      // Output the title at that list
		    << _listing[i]._duplicates << "\t"  // Output the duplicates at that list
		    << "\n";
	}

	// All is well
	return true;
}

bool SecondaryIndex::read (BinaryData objects[], unsigned int length)
{
#ifdef _DEBUG_
	printf ("Reading binary object array into secondary index.\n");
#endif // _DEBUG_

	// If length arg is 0, attempt to calculate the length using size division
	if (length == 0) {
#ifdef _DEBUG_
		printf ("Length argument 0. Attempting to calculate length from array.");
#endif // _DEBUG_

		length = sizeof(objects)/sizeof(objects[0]);
	}

	// Calculated length still zero exception.
	if (length == 0) {
#ifdef _DEBUG_
		printf ("Calculated length is zero! Exiting.\n");
#endif // _DEBUG_

		return false;
	}

	// Index exceeds buffer exception
	else if (length >= LISTING_BUFFER) {
#ifdef _DEBUG_
		printf ("Error: Length of array exceeds listing buffer of PrimaryIndex class. Exiting. Length: %d\n", length);
#endif // _DEBUG_

		return false;
	}

#ifdef _DEBUG_
	printf ("Calculated length of array: %d\n", length);
	printf ("Iterating...\n");
#endif // _DEBUG_

	// Iterate
	for (unsigned int i = 0; i < length; i++) {
		// Never add things to the index that are flagged as deleted.
		if (!objects[i].is_deleted()) {
#ifdef _DEBUG_
			printf ("Update to secondary index: Type %s\n", objects[i].type().c_str());
#endif // _DEBUG_

			update_type(objects[i].type());
		}
	}

	return true;
}

bool SecondaryIndex::type_match(std::string type)
{
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		if (_listing[i]._type == type)
			return true;
	}
	return false;
}
