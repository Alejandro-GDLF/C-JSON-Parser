#include <stdio.h>

#include "gettime.h"
#include "../headers/c_json_parser.h"

#define NUM_SAMPLES 2

char* read_file(const char* file_name);
long int get_file_size(FILE *fptr);
double make_benchmarking(char* string_to_parse);

int main()
{
    printf("Making benchmarking...\n\n");
    const char *samples_files[NUM_SAMPLES] = {"./benchmarking/sample2.json", "./benchmarking/sample2.json"};

    for (int i = 0; i < NUM_SAMPLES; i++ )
    {
        char* contents = read_file(samples_files[i]);
        if (contents == NULL )
            continue;

        printf("Contents read from %s\n", samples_files[i]);
        
        JSONRoot root;
        double start = get_time();
        if (parse_json(&root, contents) < 0 )
        {
            printf("Error in json parsing.\n");
        }
        else {
            printf("Success.\n");
        }

        double end = get_time();

        printf("Time elapsed for %s = %f\n",samples_files[i], end - start);

        free(contents);
        c_json_free(&root);
        printf("\n\n\n");
    }

    printf("Ended benchmarking.\n");

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

    fread(contents, file_size, 1, fptr);

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
        rewind(fptr);

    return size;
}