/*
    Secondary Index Class Header file
    John "Jack" Riales, CSC230, J00515747

    Class will keep track of values ("types" section), counting
    duplicate values, of the relative file, and print it through
    to a sequential file.

    Class Duties:
        Maintain an array data structure to keep track of type strings
        Keep count of said strings using the "zero record"

    Dependencies:
        BinaryData
            Standard IO
            STL String
            File Stream
*/

#ifndef SECONDARY_H
#define SECONDARY_H

#include "binary.h"

#define _DEBUG_

class SecondaryIndex
{
    public:
        SecondaryIndex (int count = 0);
        /**
            \name Constructor
            \brief Initializes attributes. Sets defaults for array data.
            \param Count what to instantiate _count as. Default is 0.
        */

        ~SecondaryIndex ();
        /**
            \name Destructor
        */

        bool set_type_at (unsigned int key, std::string type);
        /**
            \name Type index setter
            \brief Adds a type at the given listing index.
            \return False if error check prompts any of the following:
                - Key is out of range
                - Type already exists
            \param Key Key of index to assign to. Unsigned to disallow negative values.
            \param Type type name to set at the index.
        */

        void update_type (std::string type);
        /**
            \name Update type
            \brief Adds the type if it does not exist, or updates its duplication value if it does
        */

        bool write (std::fstream& out);
        /**
            \name Write
            \brief Writes the secondary index to a sequential file.
        */

        bool read (BinaryData objects[], unsigned int length = 0);
        /**
            \name Read
            \brief Reads the primary index back from the binary file.

            Performed at start.
        */

        bool type_match (std::string type);
        /**
            \name Type Match
            \brief Returns if the type was found at the given position
            \return True type was found
            \return False type was not found
            \param Type type to search for
        */

        static const unsigned int TAG_BUFFER     = 11;
        /**
            \name Tag Buffer
            \brief How many possible duplicates of a type can be held in one listing index?
        */

        static const unsigned int LISTING_BUFFER = 25;
        /**
            \name Listing Array Buffer Length
            \brief Arbitrary array length of 25, held here for more readability when iterating.
        */

    private:
        struct {                                    /// Anonymous struct used to house index
            std::string _type;                      /// Type name from the "BinaryData" class
            int         _duplicates;                /// How many duplicates of the type there are
        }               _listing[LISTING_BUFFER];   /// Array with arbitrary buffer defines index
        int             _count;                     /// How many actual entries are there?
};

#endif // SECONDARY_H
