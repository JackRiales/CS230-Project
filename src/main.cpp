/*
    Program 1 Main Source File
    John "Jack" Riales, CSC230. J00515647
*/

#include "binary.h"
#include "primary.h"
#include "secondary.h"
using namespace std;

int main (int argc, char** argv)
{
    // Testing, only if debug has been declared inside binary.h
    #define _DEBUG_
    #ifdef _DEBUG_
    // #define _TEST_1_
    // #define _TEST_2_
    // #define _TEST_3_
    #define _TEST_4_

    #ifdef _TEST_4_
    // Parsing works! Now let's integrate the primary index.
    // We need multiple binary objects. Use one for each line in input.txt -- this time 12!
    BinaryData obj[12];
    PrimaryIndex index;
    SecondaryIndex sindex;

    std::fstream test_in ("input.txt", ios::in);

    // Iterate through the object array and parse each
    for (int i = 0; i < 12; i++)
    {
        // Set values
        obj[i].read_sequential(test_in, i);
    }

    // We don't need input anymore
    test_in.close();

    std::fstream test_out ("testing.bin", ios::out | ios::binary);
    for (int i = 0; i < 12; i++)
    {
        // Push to the binary file at position i
        obj[i].write (test_out, i);
    }

    // Dont need this anymore
    test_out.close();

    // But now we have this primary index. Have it read our array. Generally this will be done after
    // we re-import our array of binary data objects.
    index.read (obj, 12);
    sindex.read (obj, 12);

    std::fstream seq_out ("Primary_Index.txt", ios::out);
    std::fstream sseq_out ("Secondary_Index.txt", ios::out);

    // Finally, let's write our primary index through the sequential output stream
    index.write (seq_out);
    sindex.write(sseq_out);

    // Close it
    seq_out.close();
    #endif

    #else // _DEBUG_
    printf ("No tests performed. _DEBUG_ macro flag is undefined.");
    #endif // _DEBUG_
    return 0;
}
