#include <stdio.h>
#include <stdlib.h>
#include "file_lib.h"
#include "wav.h"

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

// Reverses the data section of a wav file
wav_file* reverse_wav_file(wav_file* file) {

    size_t data_size = file->size - 44;
    long int num_samples = (file->size - 44) / 8;
    
    // Swap samples "symmetrically" (swap nth sample with nth-to-last sample)
    for (int i=0; i < num_samples/2; i++) {

        // Sample swap logic
        char temp[8];
        for (int j=0; j<8; j++) {
            temp[j] = file->data[i*8 + j];
            file->data[i*8 + j] = file->data[data_size - (i*8 + j) - 1];
            file->data[data_size - (i*8 + j) - 1] = temp[j];
        }
    }

    return file;
}

int main(int argc, char** argv){

    // Check for expected number of arguments
    if (argc < 3) {
        perror("Expected 2 file paths (source and destination)");
        exit(1);
    }

    // Check for source path existing
    FILE *fd = fopen(argv[1], "r");
    if (fd == NULL) {
        perror("Source file not found");
        exit(1);
    }
    fclose(fd);

    // Create new WAV file
    wav_file *file = create_wav_file(argv[1]);

    // Reverse wav file
    file = reverse_wav_file(file);

    // Save file to disk
    save_to_disk(argv[2], file, file->size);

    // Display information
    printf("Reversed \"%s\" --> \"%s\"\n\n", argv[1], argv[2]);
    printf("Sample Rate: %ld\n", bytesToDecimal(file->header->sample_rate, 4));
    printf("File Size: %ld\n", file->size);
    printf("Number of Channels: %ld\n", bytesToDecimal(file->header->channels, 2));

    // Free all allocated memory
    free(file->header);
    free(file->data);
    free(file);

    return 0;
}
