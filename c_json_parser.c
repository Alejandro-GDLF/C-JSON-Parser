#include <stdio.h>
#include <stdlib.h>

#include "c_json_parser.h"

const char STRING_ENCAPSULATOR = '"';

/********************* parse_string *********************
 * @brief parses a substring inside a char array encapsulated inside " chars
 * 
 * @param substring substring extracted
 * @param substr_length length of the substring
 * @param string_to_parse pointer to a " character that contains a string
 * @param string_to_parse_length length of the string passed
 * 
 * @return 0 success
 * @return -1 error has occured
*/

char* parse_string(char* substring, int* substr_length, char* string_to_parse, size_t string_to_parse_length) {
    if( string_to_parse[0] != STRING_ENCAPSULATOR ) return -1;

    *substr_length = strchr(string_to_parse + 1, (int) STRING_ENCAPSULATOR ) - string_to_parse;
    substring = (char*) malloc((*substr_length) + 1);

    strncpy(substring, string_to_parse + 1, (*substr_length) - 1);
    substring[*substr_length] = '\0';

    return 0;
}

void* parse(char* string_to_parse, size_t string_to_parse_length) {
    void* root;

    // ! Not fully implemented yet

    for (int i = 0; i < string_to_parse_length; i++ ) {
        char current_char = string_to_parse[i];

        if( current_char == '{') {
            root = parse_object( string_to_parse + i, string_to_parse_length - i );
            continue;
        }

        else if( current_char == '[' ) {
            root = parse_array( string_to_parse + i,  string_to_parse_length - i );
            continue;
        }

        else if( isdigit(current_char) ) {
            root = parse_number( string_to_parse + i,  string_to_parse_length - i );
            continue;
        }

        else if( current_char == STRING_ENCAPSULATOR){
            root = parse_string( string_to_parse + i, string_to_parse_length - i);
            continue;
        }
    }

    return root;
}

JSONRoot* parse_json_string(char* string_to_parse, size_t string_to_parse_length) {
    return (JSONRoot*) parse(string_to_parse, string_to_parse_length);
}

void* get_value(JSONObject* json_object, char* field_name, JSONType* value_type, void* error){
    // TODO: Check if json_object is set

    // TODO: Search for key "field_name" in json_object

    // TODO: Handle errors

    // TODO: Clean up

    // TODO: Return value or NULL if error occurred
}