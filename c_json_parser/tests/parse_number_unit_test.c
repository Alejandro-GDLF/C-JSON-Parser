#include <stdio.h>

#include "../headers/c_json_parser.h"

#define N 4

void make_integer_test(char strings[N][100], int numbers[N], JSONType type, char* id) {

    for( int i = 0; i < N; i++ )
    {
        JSONRoot root;

        if( parse(&root, strings[i], NULL )) 
        {
            printf("Unit test: parse_number_unit_test failed at %s.\nparse returned -1\n", id);
            continue;
        }

        if( root.type != type ) 
        {
            printf("Unit test: parse_number_unit_test failed at %s.\nIncorrect type: %d\n\t-String processed: %s.\n\t-Number recieved: %d.\n", id, root.type, strings[i], numbers[i]);
            continue;
        }

        if( root.value.integer_value != numbers[i] )
        {
            printf("Unit test: parse_number_unit_test failed at %s.\nIncorrect value.\n\t-String processed: %s.\n\t-Number recieved: %d.\n", id, strings[i], numbers[i]);
            continue;
        }

        printf("Unit test: parse_number_unit_test %s successful.\n", id);
    }
}

void make_double_test(char strings[N][100], double numbers[N], JSONType type, char* id) {

    for( int i = 0; i < N; i++ )
    {
        JSONRoot root;

        if( parse(&root, strings[i], NULL )) 
        {
            printf("Unit test: parse_number_unit_test failed at %s.\nparse returned -1\n", id);
            continue;
        }

        if( root.type != type ) 
        {
            printf("Unit test: parse_number_unit_test failed at %s.\nIncorrect type: %d\n\t-String processed: %s.\n\t-Number recieved: %f.\n", id, root.type, strings[i], numbers[i]);
            continue;
        }

        if( root.value.float_value != numbers[i] )
        {
            printf("Unit test: parse_number_unit_test failed at %s.\nIncorrect value.\n\t-String processed: %s.\n\t-Number recieved: %f.\n", id, strings[i], numbers[i]);
            continue;
        }

        printf("Unit test: parse_number_unit_test %s successful.\n", id);
    }
}

void parse_number_integer_unit_test() 
{
    char strings[N][100] = { "12", "    32", "23456", "12" };
    int response[N] = {12, 32, 23456, 12};

    make_integer_test(strings, response, Integer, "Integer_test");
}

void parse_number_double_unit_test() 
{
    char strings[N][100] = { "12.2", "    32.3", "23456.1", "12.4" };
    double response[N] = {12.2, 32.3, 23456.1, 12.4};

    make_double_test(strings, response, Float, "Float_test");
}

void run_parse_number_unit_tests()
{
    parse_number_integer_unit_test();
    parse_number_double_unit_test();
}