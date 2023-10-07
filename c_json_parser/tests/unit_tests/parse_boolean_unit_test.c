#include <stdio.h>

#include "c_json_parser.h"
#include "unit_tests.h"

#define N 4

void make_boolean_test(char strings[N][20], int numbers[N], char* id) 
{

    for( int i = 0; i < N; i++ )
    {
        JSONRoot root;

        if( parse_json(&root, strings[i])) 
        {
            printf("Unit test: parse_boolean_unit_test failed at %s.\nparse returned -1\n", id);
            continue;
        }

        if( root.type != Boolean ) 
        {
            printf("Unit test: parse_boolean_unit_test failed at %s.\nIncorrect type: %d\n\t-String processed: %s.\n\t-Number recieved: %d.\n", id, root.type, strings[i], numbers[i]);
            continue;
        }

        if( root.value.boolean_value != numbers[i] )
        {
            printf("Unit test: parse_boolean_unit_test failed at %s.\nIncorrect value.\n\t-String processed: %s.\n\t-Number recieved: %d.\n", id, strings[i], numbers[i]);
            continue;
        }

        printf("Unit test: parse_boolean_unit_test %s successful.\n", id);
    }
}

void parse_boolean_unit_test()
{
    char strings[N][20] = {"true", "false", "     true", "    false"};
    int res[N] = {1, 0, 1, 0};

    make_boolean_test(strings, res, "boolean_unit_tests");
}


void run_parse_boolean_unit_tests()
{
    parse_boolean_unit_test();
}