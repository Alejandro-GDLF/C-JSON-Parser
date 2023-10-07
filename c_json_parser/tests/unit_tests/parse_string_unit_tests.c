#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "c_json_parser.h"
#include "unit_tests.h"

void parse_string_unit_test() 
{
    JSONRoot root;

    char input[] = "\"helloWorld\"";
    char expected_output[] = "helloWorld";

    if( parse_json((JSONValue*)&root, input) == -1) {
        printf("Unit test: parse_string_unit_test failed.\nparse returned -1\n");
        goto finish;
    }

    if( root.type != String) {
        printf("Unit test: parse_string_unit_test failed.\nIncorrect type");
    }

    if( strcmp( root.value.string_value, expected_output) != 0) {
        printf("Unit test: parse_string_unit_test failed.\nString comparison failed:\n-String expected: %s\n-String recieved:%s\n", root.value.string_value, expected_output);
        goto finish;
    }

    printf("Unit test: parse_string_unit_test successful.\n");

    finish:
        free(root.value.string_value);
}

void parse_string_unit_test_failed ()
{
    JSONRoot root;

    char input[] = "\"kjsnf";
    if( parse_json(&root, input) == -1) 
    {
        printf("Unit test: parse_string_unit_test_failed sucessful.\n");
        goto finish;
    }

    printf("Unit test: parse_string_unit_test_failed failed.\n");

    finish:
        free(root.value.string_value);
}

void parse_string_unit_test_spaces()
{
    JSONRoot root;

    char input[] = "            \"helloWorld\"";
    char expected_output[] = "helloWorld";

    if( parse_json((JSONValue*)&root, input) == -1) {
        printf("Unit test: parse_string_unit_test_spaces failed.\nparse returned -1\n");
        goto finish;
    }

    if( root.type != String) {
        printf("Unit test: parse_string_unit_test_spaces failed.\nIncorrect type");
    }

    if( strcmp( root.value.string_value, expected_output) != 0) {
        printf("Unit test: parse_string_unit_test_spaces failed.\nString comparison failed:\n-String expected: %s\n-String recieved:%s\n", root.value.string_value, expected_output);
        goto finish;
    }

    printf("Unit test: parse_string_unit_test_spaces successful.\n");

    finish:
        free(root.value.string_value);
}

void parse_string_unit_test_tabs()
{
    JSONRoot root;

    char input[] = "\t\t\"helloWorld\"";
    char expected_output[] = "helloWorld";

    if( parse_json((JSONValue*)&root, input) == -1) {
        printf("Unit test: parse_string_unit_test_tabs failed.\nparse returned -1\n");
        goto finish;
    }

    if( root.type != String) {
        printf("Unit test: parse_string_unit_test_tabs failed.\nIncorrect type");
    }

    if( strcmp( root.value.string_value, expected_output) != 0) {
        printf("Unit test: parse_string_unit_test_tabs failed.\nString comparison failed:\n-String expected: %s\n-String recieved:%s\n", root.value.string_value, expected_output);
        goto finish;
    }

    printf("Unit test: parse_string_unit_test_tabs successful.\n");

    finish:
        free(root.value.string_value);
}

void parse_string_unit_test_escaped_double_quotes() {
    JSONRoot root;

    char input[] = "\"hello\\\"World\\\"\"";
    char expected_output[] = "hello\"World\"";

    if( parse_json((JSONValue*)&root, input) == -1) {
        printf("Unit test: parse_string_unit_test_scaped_double_quotes failed.\nparse returned -1\n");
        goto finish;
    }

    if( root.type != String) {
        printf("Unit test: parse_string_unit_test_scaped_double_quotes failed.\nIncorrect type");
    }

    if( strcmp( root.value.string_value, expected_output) != 0) {
        printf("Unit test: parse_string_unit_test_scaped_double_quotes failed.\nString comparison failed:\n-String input: %s\n-String recieved: %s\n-String expected: %s\n", input, root.value.string_value, expected_output);
        goto finish;
    }

    printf("Unit test: parse_string_unit_test_scaped_double_quotes successful.\n");

    finish:
        free(root.value.string_value);
}

void run_parse_string_unit_tests() 
{
    parse_string_unit_test_tabs();
    parse_string_unit_test_spaces();
    parse_string_unit_test_failed ();
    parse_string_unit_test();
    parse_string_unit_test_escaped_double_quotes();
}