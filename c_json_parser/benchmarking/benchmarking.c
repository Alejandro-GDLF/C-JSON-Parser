#include <stdio.h>

#include "gettime.h"
#include "../headers/c_json_parser.h"

#define NUM_SAMPLES 2

char* read_file(const char* file_name);
long int get_file_size(FILE *fptr);
double make_benchmarking(char* string_to_parse);

int main()
{
    const char *samples_files[NUM_SAMPLES] = {"./benchmarking/sample1.json", "./benchmarking/sample2.json"};

    for (int i = 0; i < NUM_SAMPLES; i++ )
    {
        printf("File_name: %s\n", samples_files[i]);
        char* contents = read_file(samples_files[i]);
        if (contents == NULL )
            continue;

        printf("%s\n\n", contents);

        free(contents);
    }

    return 0;
}

char* read_file(const char* file_name)
{
    FILE *fptr;
    long int file_size;
    const char *mode = "r";

    fptr = fopen(file_name, mode);
    if( fptr == NULL )
    {
        perror("Error at opening a file\n");
        return NULL;
    }

    file_size = get_file_size(fptr);

    char* contents = malloc(file_size);

    fgets(contents, file_size + 1, fptr);

    fclose(fptr);

    return contents;
}

long int get_file_size(FILE *fptr)
{
    fpos_t pos;
    long int size;

    // Get current position in stream
    int status_code = fgetpos(fptr, &pos);

    // Get size
    fseek(fptr, 0, SEEK_END);
    size = ftell(fptr);

    if( status_code == 0 )
        fsetpos(fptr, &pos);

    else
        fseek(fptr, 0, SEEK_SET);

    return size;
}