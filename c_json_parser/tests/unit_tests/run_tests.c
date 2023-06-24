#include <stdio.h>

#include "unit_tests.h"

int main() 
{
    printf("--------- PARSE STRING UNIT TEST ---------------\n");
    run_parse_string_unit_tests();

    printf("\n\n--------- PARSE NUMBER UNIT TEST ---------------\n");
    run_parse_number_unit_tests();

    printf("\n\n------------- PARSE BOOLEAN UNIT TESTS -----------------\n");
    run_parse_boolean_unit_tests();

    printf("\n\n---------------PARSE ARRAY UNIT TESTS ------------------\n");
    run_parse_array_unit_tests();

    printf("\n\n---------------PARSE OBJECT UNIT TESTS ------------------\n");
    run_parse_object_unit_tests();

    return 0;
}