#include <stdio.h>
#include <stdlib.h>
#include "file_lib.h"

/**
 * The contents of the file are a char*, but that doesn't
 * mean it is a "string".  When working with "strings" in C
 * though, we normally NULL terminate.  This is not NULL
 * terminated.  Nor could it be, as we then wouldn't be
 * able to represent NULL in the file.  So, we need to print
 * each character separately.  Here, we are printing as bytes
 * (base 16).
 * 
 * @parameters
 * char* file_contents - The file's bytes.
 * size_t num_bytes - The number of bytes to print.
 */
void print_file(char* file_contents, size_t num_bytes){
    // Note this is printing the bytes in hex
    for(size_t i = 0; i<num_bytes; ++i){
        printf("%x", file_contents[i]);
    }
    printf("\n");
}

int main(int argc, char** argv){
    char* contents;

    // Read the input file
    size_t num_bytes = read_file(argv[1], &contents);
    printf("%s\n", argv[1]);
    printf("%lu bytes read.\n", num_bytes);

    // Print the file contents in hex
    print_file(contents, num_bytes);

    // Write the file to the output
    write_file(argv[2], contents, num_bytes);

    free(contents); // Always free allocated memory
    return 0;
}
