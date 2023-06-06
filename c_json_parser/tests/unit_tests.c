#include <stdio.h>
#include <assert.h>

#include "../src/c_json_parser.h"

void parse_string_unit_test() {
    JSONRoot root;

    char input[] = "\"helloWorld\"";
    char expected_output[] = "helloWorld";

    if( parse(root, input) == -1) {
        printf("Unit test: parse_string_unit_test failed.\nparse returned -1\n");
        return;
    }

    if( strcmp( (char*)root, expected_output) != 0 ) {
        printf("Unit test: parse_string_unit_test failed.\nString comparison failed:\n-String expected: %s\n-String recieved:%s\n", (char*)root, expected_output);
        return;
    }

    printf("Unit test: parse_string_unit_test successful.\n");
}

int main() {
    parse_string_unit_test();

    return 0;
}