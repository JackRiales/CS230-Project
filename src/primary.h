/*
    Primary Index Class Header File
    John "Jack" Riales, CSC230. J00515647

    Class will keep track of unique values (the "titles" section)
    of the relative file, and print it through to a sequential file.

    Class Duties:
        Maintain an array data structure to keep track of title strings.

    Dependencies:
        BinaryData
            Standard IO
            STL String
            File stream
*/

#ifndef PRIMARY_H
#define PRIMARY_H

#include "binary.h"

#define _DEBUG_

class PrimaryIndex
{
    public:
        PrimaryIndex (int count = 0);
        /**
            \name Constructor
            \brief Initializes attributes. Sets defaults for array data.
            \param Count what to instantiate _count as. Default is 0.
        */

        ~PrimaryIndex ();
        /**
            \name Destructor.
        */

        bool set_title_at (unsigned int key, std::string title);
        /**
            \name Title index setter
            \brief Adds title at the given key index.
            \return False if error check prompts any of the following:
                - Key is out of range of the index buffer
                - Title already exists
            \return True if no errors are detected.
            \param Key Key of index to assign to. Is unsigned int to take care of negative key value problems.
            \param Title title to set at the index.

            Returns false if the title is already set at the index because we don't want users
            accidentally overwriting titles all over the place.
        */

        bool change_title_to (unsigned int key, std::string title);
        /**
            \name Title index change
            \brief Changes title at the given key index, if it exists.
            \return False if error checking prompts any of the following:
                - Key is out of range of the index buffer
                - Title does not already exist
            \return True no errors are detected
            \param Key key of index to assign to. Unsigned to take care of negative index issue.
            \param Title replacement title

            "Change" is separated from "set_title_at" because the end user might not always know
            what they are changing if they "set_title_at" a preexisting title without realizing it.
        */

        bool write (std::fstream& out);
        /**
            \name Write
            \brief Writes the primary index to a sequential file.
        */

        bool read (BinaryData objects[], unsigned int length = 0);
        /**
            \name Read
            \brief Creates a primary index based on an array of data objects
            \param Objects Array of binary objects
            \param Length Length of the array. If zero, will attempt to use byte sizing to find length.

            Performed at start.
        */

        const int indexOf (std::string title) const;
        /**
            \name Find index of title
            \brief Returns the index of the given title.
            \return -1 if not found. Else returns index. Read only.
        */

        const std::string titleAt (unsigned int key) const;
        /**
            \name Find title at index
            \brief Returns the title at the given index.
            \return "0" for any of the following reasons:
                - Key out of bounds of index buffer
                - Value at key is blank (in which case, it would return this anyway...)
            \return Title at index otherwise. Read only.
        */

        bool title_exists (std::string title);
        /**
            \name Title Exists
            \brief Returns whether or not there is a title set yet in the index.
            \return False no title was found
            \return True title was found somewhere

            Effectively a boolean wrapper function for "indexOf." If -1 is returned, return false.
            Used as a slightly easier to write way of determining whether or not to write a title.
        */

        static const unsigned int LISTING_BUFFER = 25;
        /**
            \name Listing Array Buffer Length
            \brief Arbitrary array length of 25, held here for more readability when iterating.
        */

    private:
        struct {                                    /// Anonymous struct used to house array data
            std::string _title;                     /// Title fields from "BinaryData" class
            int         _tag;                       /// Positioning tag
        }               _listing [LISTING_BUFFER];  /// Array with arbitrary buffer defines index
        int             _count;                     /// How many actual entries are there?
};

#endif // PRIMARY_H
