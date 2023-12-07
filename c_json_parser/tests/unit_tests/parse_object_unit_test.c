#include <stdio.h>

#include "c_json_parser.h"
#include "unit_tests.h"
#define N 8

void make_object_test(size_t expected, char input[])
{
    printf("\nTest: %s\n\n", input);
    JSONRoot root;
    if( parse_json(&root, input) == -1 )
    {
        printf("Unit test: test failed. Parse returned -1\n");
        return;
    }

    if( root.type != JsonObject )
    {
        printf("Unit test: test failed. Parse returned wrong type: %d.\n", JsonObject);
        printf("\t- Returned: %d\n", root.type);
        return;
    }

    if ( root.value.json_object.entries_length != expected )
    {
        printf("Unit test: test failed. Array length wrong.\n");
        printf("\t- Recieved: %d.\n\t-Expected: %d.\n", root.value.json_object.entries_length, expected);
        return;
    }

    printf("Unit test: test passed successfully.\n");
}

void unit_test_parse_object()
{
    char input[N][100] = { "{  }", "{\"something\":3}", "{\"3\":32,\"res\":23}", "{ \"44wfc\":[],\"r1vrve\":[3,2,1] }", "{\"res\": {}}", "{\"fe\":\"fr\", \"ce\": \"fr\"}", "{\"res\": {\"something\":3}}", "{\"Some\":{\"Some\":{\"Some\":23}}}"};
    size_t expected[N] = {0, 1, 2, 2, 1, 2, 1, 1};

    for( int i = 0; i < N; i++ )
        make_object_test(expected[i], input[i]);

}

void run_parse_object_unit_tests()
{
    unit_test_parse_object();
}