#include <stdio.h>
#include <stdlib.h>

#include "c_json_parser.h"

char ENCLOSING_FIELDS_CHAR = '"';

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

        else if( current_char == '"'){
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