#include "wav.h"
#include "file_lib.h"
#include <string.h>
#include <stdint.h>

// Converts little endian bytes to decimal
long int bytesToDecimal(unsigned char *bytes, size_t num_bytes) {
    long int result = 0;
    long int multiplier = 1;
    for (int i=0; i<num_bytes; i++) {
        result += bytes[i] * multiplier;
        multiplier *= 256;
    }
    return result;
}

// Creating WAV header struct
wav_header* create_header(char* contents, size_t num_bytes) {
    
    wav_header *h = malloc(44);
    int i, j;

    // Validate subtype ("RIFF")
    for (i=0, j=0; i<4; i++, j++) {
        h->subtype[j] = contents[i];
    }
    if (h->subtype[0] != 'R' || h->subtype[1] != 'I' || h->subtype[2] != 'F' || h->subtype[3] != 'F') { // strcmp() doesn't work, I apologize for this monstrosity
        perror("Unexpected file subtype");
        exit(1);
    }

    // Validate file size - 8
    for (i=4, j=0; i<8; i++, j++) {
        h->bytes[j] = contents[i];
    }
    if (bytesToDecimal(h->bytes, 4) != num_bytes - 8) {
        perror("Unexpected file size");
        exit(1);
    }

    // Validate type ("WAVE")
    for (i=8, j=0; i<12; i++, j++) {
        h->type[j] = contents[i];
    }
    if (h->type[0] != 'W' || h->type[1] != 'A' || h->type[2] != 'V' || h->type[3] != 'E') {
        perror("Unexpected file type");
        exit(1);
    }

    // Validate format ("fmt")
    for (i=12, j=0; i<16; i++, j++) {
        h->format[j] = contents[i];
    }
    if (h->format[0] != 'f' || h->format[1] != 'm' || h->format[2] != 't') {
        perror("Unexpected format chunk");
        exit(1);
    }

    // Assign format length
    for (i=16, j=0; i<20; i++, j++) {
        h->format_length[j] = contents[i];
    }

    // Validate format type (0x0100)
    for (i=20, j=0; i<22; i++, j++) {
        h->format_type[j] = contents[i];
    }
    if (bytesToDecimal(h->format_type, 2) != 1) {
        perror("Unexpected format type");
        exit(1);
    }

    // Validate number of channels (2)
    for (i=22, j=0; i<24; i++, j++) {
        h->channels[j] = contents[i];
    }
    if (bytesToDecimal(h->channels, 2) != 2) {
        perror("Unexpected number of channels");
        exit(1);
    }

    // Assign sample rate
    for (i=24, j=0; i<28; i++, j++) {
        h->sample_rate[j] = contents[i];
    }

    // Assign bits per second
    for (i=28, j=0; i<32; i++, j++) {
        h->bits_per_second[j] = contents[i];
    }

    // Assign data part that confuses me
    for (i=32, j=0; i<34; i++, j++) {
        h->not_sure_tbh[j] = contents[i];
    }

    // Assign bits per sample
    for (i=34, j=0; i<36; i++, j++) {
        h->bits_per_sample[j] = contents[i];
    }

    // Validate data header ("data")
    for (i=36, j=0; i<40; i++, j++) {
        h->data[j] = contents[i];
    }
    if (h->data[0] != 'd' || h->data[1] != 'a' || h->data[2] != 't' || h->data[3] != 'a') {
        perror("Unexpected data header");
        exit(1);
    }

    // Assign data size
    for (i=40, j=0; i<44; i++, j++) {
        h->size[j] = contents[i];
    }

    return h;
}

// Takes a pathname, reads from the specified file, and creates a copied WAV file from it
wav_file* create_wav_file(char* path) {

    // Read the input file, set size of wav_file
    char* contents;
    size_t num_bytes = read_file(path, &contents);

    // Allocate memory for wav_file, assign file size
    wav_file *file = malloc(num_bytes);
    file->size = num_bytes;
    
    // Create WAV header, set header pointer of wav_file
    file->header = create_header(contents, num_bytes);

    // Set data pointer of wav_file
    file->data = malloc(num_bytes - 44);
    for (int i=44, j=0; i<num_bytes; i++, j++) {
        file->data[j] = contents[i];
    }

    // Free memory
    free(contents);

    return file;
}

// Takes a path and wav file and saves the wav file at the specified path
void save_to_disk(char* path, wav_file* file, size_t size) {
    
    // raw bytes file
    char *data = malloc(size);
    
    // write header and data to file
    memcpy(data, file->header, 44);
    memcpy(data + 44, file->data, size - 44);

    // save file contents to disk
    if (write_file(path, data, size) < size) {
        perror("Error writing file to disk");
        free(data);
        exit(1);
    }

    free(data);
}