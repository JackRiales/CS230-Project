/*
    Primary Index Class Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "primary.h"
using namespace std;

// Constructor
PrimaryIndex::PrimaryIndex(int count):
	_count (count)
{
#ifdef _DEBUG_
	printf("Instantiating a new primary index with count value %d\n", count);
#endif

	// Iterate through the listing and instantiate with default values
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		_listing[i]._title = "0";
		_listing[i]._tag = 0;
	}
}

// Destructor
PrimaryIndex::~PrimaryIndex()
{
	; // Nothing to see here... yet!
}

bool PrimaryIndex::set_title_at(unsigned int key, std::string title)
{
	// Error check the key, first
	if (key >= LISTING_BUFFER) {
#ifdef _DEBUG_
		printf("Error! Given key exceeds index buffer range of primary index.\n");
#endif

		return false;
	} else {
		if (!title_exists(title)) {
#ifdef _DEBUG_
			printf("Setting title at given key %d to %s\n", key, title.c_str());
#endif

			_listing[key]._title = title;
		} else {
#ifdef _DEBUG_
			printf ("Title %s already exists in the primary index. Index found was: %d\n", title.c_str(), indexOf(title));
#endif

			return false;
		}
	}
	return true;
}

bool PrimaryIndex::change_title_to(unsigned int key, std::string title)
{
	// Error check the key, first
	if (key >= LISTING_BUFFER) {
#ifdef _DEBUG_
		printf("Error! Given key exceeds index buffer range of primary index.\n");
#endif

		return false;
	} else {
		if (title_exists(title)) {
#ifdef _DEBUG_
			printf("Setting title at given key %d to %s\n", key, title.c_str());
#endif

			_listing[key]._title = title;
		} else {
#ifdef _DEBUG_
			printf ("Title %s does not already exist in the primary index. Suggestion: PrimaryIndex::set_title_at(%d, %s)\n", title.c_str(), key, title.c_str());
#endif

			return false;
		}
	}
	return true;
}

bool PrimaryIndex::write (fstream& out)
{
	// Error check stream
	if (!out) {
#ifdef _DEBUG_
		printf ("Output stream is not open. Please check your parameters.\n");
#endif // _DEBUG_

		return false;
	}

#ifdef _DEBUG_
	printf ("Writing Primary Index to output stream.\n");
#endif // _DEBUG_

	// Header
	out << "Listing #\tTitle\t\tTag\t";

	// Iterate through listing
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		// Output; one line per listing
		out << "\n"                         // New line
		    << i << "\t\t"                  // Output the position of the list we're at
		    << _listing[i]._title << "\t\t"   // Output the title at that list
		    << _listing[i]._tag << "\t"     // Output the tag at that list
		    << "\n"                         // New line
		    ;
	}

	// All is well
	return true;
}

bool PrimaryIndex::read (BinaryData objects[], unsigned int length)
{
#ifdef _DEBUG_
	printf ("Reading binary object array into primary index.\n");
#endif // _DEBUG_

	// Get the size of the array
	if (length == 0)
		length = sizeof(objects)/sizeof(objects[0]);

	if (length >= LISTING_BUFFER) {
#ifdef _DEBUG_
		printf ("Error: Length of array exceeds listing buffer of PrimaryIndex class. Exiting. Length: %d\n", length);
#endif // _DEBUG_

		return false;
	} else if (length == 0) {
#ifdef _DEBUG_
		printf ("Length is zero! Exiting.\n");
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
			printf ("Added to primary index: Title %s at Tag %d\n", objects[i].title().c_str(), i);
#endif // _DEBUG_

			set_title_at(i, objects[i].title());
			_listing[i]._tag = i; // ?
		}
	}

	return true;
}

const int PrimaryIndex::indexOf(std::string title) const
{
	for (unsigned int i = 0; i < LISTING_BUFFER; i++) {
		if (_listing[i]._title == title)
			return i;
	}

	// If unfound, return negative index
	return -1;
}

const std::string PrimaryIndex::titleAt(unsigned int key) const
{
	// Error check the key, first
	if (key >= LISTING_BUFFER) {
#ifdef _DEBUG_
		printf("Error! Given key exceeds index buffer range of primary index.");
#endif // _DEBUG_
		return "0";
	} else {
		// Return listing if not "0"
		if (_listing[key]._title != "0") {
			return _listing[key]._title;
		} else {
#ifdef _DEBUG_
			printf ("Warning: Title at index is '0'.");
#endif // _DEBUG_
		}
	}
	return "0";
}

bool PrimaryIndex::title_exists(std::string title)
{
	// If the index returns not found or the title at said index is "0", there is no title.
	if (indexOf(title) == -1 || titleAt(indexOf(title)) == "0")
		return false;

	// Otherwise, naturally, there is
	else return true;
}
