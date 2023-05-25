#include <stdio.h>

#include "c_json_parser.h"


JSONRoot* parse_json_string(char* string_to_parse, size_t string_to_parse_length) {
    // TODO: detect type

    // TODO: parse type

    // TODO: Recursivly parse nested types

}

void* get_value(JSONObject* json_object, char* field_name, JSONType* value_type, void* error){
    // TODO: Check if json_object is set

    // TODO: Search for key "field_name" in json_object

    // TODO: Handle errors

    // TODO: Clean up

    // TODO: Return value or NULL if error occurred
}