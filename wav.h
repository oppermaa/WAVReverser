#ifndef WAV_H
#define WAV_H

#include <stdlib.h>
#include <stdio.h>

// WAV header struct
typedef struct {
    unsigned char subtype[4];
    unsigned char bytes[4];
    unsigned char type[4];
    unsigned char format[4];
    unsigned char format_length[4];
    unsigned char format_type[2];
    unsigned char channels[2];
    unsigned char sample_rate[4];
    unsigned char bits_per_second[4];
    unsigned char not_sure_tbh[2];
    unsigned char bits_per_sample[2];
    unsigned char data[4];
    unsigned char size[4];
} wav_header;

// WAV file struct
typedef struct {
    wav_header *header;
    unsigned long size;
    char *data;
} wav_file;

// Converts little endian byte values of size_t to decimal
long int bytesToDecimal(unsigned char *, size_t);

// Given a WAV file in raw bytes, creates a header
wav_header* create_header(char *, size_t);

// Given a path, reads from a WAV file and creates wav_header and wav_file structs
wav_file* create_wav_file(char *);

// Given a WAV file and a path, writes it to the path
void save_to_disk();

#endif
