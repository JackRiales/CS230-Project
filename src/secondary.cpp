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
    printf("Instantiating a new secondary index with count value %d", count);
    #endif

    // Iterate and instantiate with default values
    for (unsigned int i = 0; i < LISTING_BUFFER; i++)
    {
        _listing[i]._type = "0";
        for (unsigned int k = 0; k < TAG_BUFFER; k++)
        {
            _listing[i]._tag[k] = 0;
        }
    }
}

SecondaryIndex::~SecondaryIndex()
{
    ; // Nothing to see here... yet!
}

bool SecondaryIndex::set_type_at(unsigned int key, std::string type)
{
    if (key >= LISTING_BUFFER)
    {
        #ifdef _DEBUG_
        printf("Error! Given key exceeds index buffer range of primary index.");
        #endif

        return false;
    }
    else
    {
        if (!type_match(type, new int))
        {
            #ifdef _DEBUG_
            printf("Setting type at given key %d to %s", key, type.c_str());
            #endif

            _listing[key]._type = type;
        }
        else
        {
            #ifdef _DEBUG_
            printf ("Type already exists in the secondary index. Suggestion: SecondaryIndex::update_type(string,int[])");
            #endif

            return false;
        }
    }
    return true;
}

void SecondaryIndex::update_type(std::string type)
{
    // Look for existing types
    for (unsigned int i = 0; i < LISTING_BUFFER; i++)
    {
        if (_listing[i]._type == type)
        {
            for (unsigned int k = 0; k < TAG_BUFFER; k++)
            {
                if (_listing[i]._tag[k] == 0)
                {
                    _listing[i]._tag[k] = 1;
                    return;
                }
            }
        }
    }

    // None of this type found, make a new entry
    for (unsigned int i = 0; i < LISTING_BUFFER; i++)
    {
        if (_listing[i]._type == "0")
        {
            _listing[i]._type = type;
        }
    }
}

bool SecondaryIndex::write (std::fstream& out)
{
    // Error check stream
    if (!out)
    {
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
    for (unsigned int i = 0; i < LISTING_BUFFER; i++)
    {
        // Get number of duplicates
        int dupes = TAG_BUFFER;
        for (int k = 0; k < TAG_BUFFER; k++)
        {
            if (_listing[i]._tag[k] == 0)
                dupes--;
            else break;
        }

        // Output; one line per listing
        out << "\n"                         // New line
            << i << "\t\t"                  // Output the position of the list we're at
            << _listing[i]._type << "\t\t"  // Output the title at that list
            << dupes << "\t"     // Output the tag at that list
            << "\n"                         // New line
            ;
    }

    // All is well
    return true;
}

bool SecondaryIndex::read (BinaryData objects[], unsigned int length)
{
    #ifdef _DEBUG_
    printf ("Reading binary object array into primary index.\n");
    #endif // _DEBUG_

    // Get the size of the array
    if (length == 0)
        length = sizeof(objects)/sizeof(objects[0]);

    if (length >= LISTING_BUFFER)
    {
        #ifdef _DEBUG_
        printf ("Error: Length of array exceeds listing buffer of PrimaryIndex class. Exiting. Length: %d\n", length);
        #endif // _DEBUG_

        return false;
    }
    else if (length == 0)
    {
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
    for (unsigned int i = 0; i < length; i++)
    {
        // Never add things to the index that are flagged as deleted.
        if (!objects[i].is_deleted())
        {
            #ifdef _DEBUG_
            printf ("Added to primary index: Title %s at Tag %d\n", objects[i].title().c_str(), i);
            #endif // _DEBUG_

            update_type(objects[i].type());
        }
    }

    return true;
}

bool SecondaryIndex::type_match(std::string type, int position[])
{
    for (unsigned int i = 0; i < LISTING_BUFFER; i++)
    {
        if (_listing[i]._type == type)
            return true;
    }
    return false;
}
