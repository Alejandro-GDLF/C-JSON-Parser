#include <stdio.h>

#include "../headers/c_json_parser.h"

#define N 4

void make_item_test(char strings[N][20], char expected_names[N][20], int expected_value[N], char* id) 
{

    for( int i = 0; i < N; i++ )
    {
        JSONEntry entry;

        if( parse_item(&entry, strings[i], NULL )) 
        {
            printf("Unit test: parse_item_unit_test failed at %s.\nparse returned -1\n", id);
            continue;
        }

        if( strcmp(entry.name, expected_names[i]) != 0 ) 
        {
            printf("Unit test: parse_item_unit_test failed at %s.\nIncorrect name: %s\n\t- String expected: %s.\n", id, entry.name, expected_names[i]);
            continue;
        }

        if( entry.value.value.integer_value != expected_value[i] )
        {
            printf("Unit test: parse_item_unit_test failed at %s.\nIncorrect value: %d.\n\t- Value expected: %d.\n", id, (int)entry.value.value.integer_value, expected_value[i]);
            continue;
        }

        printf("Unit test: parse_item_unit_test %s successful.\n", id);
    }
}

void parse_item_unit_test()
{
    char strings[N][20] = {"\"test1\": 23 ,", "\"test2\": 1234 ,", "\"test3\": 12314 ,", "\"test4\": 785 ,"};
    char names[N][20] = {"test1", "test2", "test3", "test4"};
    int res[N] = {23, 1234, 12314, 785};

    make_item_test(strings, names, res, "item_unit_test");
}

void run_parse_item_unit_tests()
{
    parse_item_unit_test();
}