#include <stdio.h>

#include "c_json_parser.h"
#include "unit_tests.h"
#define N 5

void make_array_test(size_t expected, char input[])
{
    JSONRoot root;
    if( parse_json(&root, input) == -1 )
    {
        printf("Unit test: test failed. Parse returned -1\n");
        return;
    }

    if( root.type != Array )
    {
        printf("Unit test: test failed. Parse returned wrong type: %d.\n", root.type);
        printf("\t- Returned: %d\n", root.type);
        return;
    }

    if ( root.value.array.length != expected )
    {
        printf("Unit test: test failed. Array length wrong.\n");
        printf("\t- Recieved: %d.\n\t-Expected: %d.\n", root.value.array.length, expected);
        return;
    }

    printf("Unit test: test passed successfully.\n");
}

void unit_test_parse_array()
{
    char input[N][50] = { "[  ]", "[ 23, 11 ]", "[1,2,3,4,5,6,7,8,9]", "[ \"somethin\", \"eres\"]", "[{},{}]"};
    size_t expected[N] = {0, 2, 9, 2, 2};

    for( int i = 0; i < N; i++ )
        make_array_test(expected[i], input[i]);

}

void run_parse_array_unit_tests()
{
    unit_test_parse_array();
}