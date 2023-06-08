#include <stdio.h>
#include <stdlib.h>

#include "c_json_parser.h"

const char STRING_ENCAPSULATOR = '"';
const char ARRAY_OPEN_ENCAPSULATOR = '[';
const char ARRAY_CLOSE_ENCAPSULATOR = ']';

/********************* parse_string *******************//**
 * @brief parses a substring.
 * @details This functions recieves a pointer to a string 
 * encapsulated in " chars like so: "<string to parse>".
 * 
 * @pre string_to_parse must be pointing to a " character.
 * @post string encapsulated with null char appended and pointer to the end of the string.
 * 
 * @param substring substring extracted.
 * @param substr_length length of the substring.
 * @param begin_string pointer to a " character that contains a string.
 * @param end_string pointer to the last " character.
 * 
 * @return 0 success
 * @return -1 error has occured
*//********************************************************/

int parse_string( char* substring, int* substr_length, char* begin_string, char* end_string ) {
    // Pointer passed must be pointing to a STRING_ENCAPSULATOR character
    if( begin_string[0] != STRING_ENCAPSULATOR ) return -1;

    end_string = strchr(begin_string + 1, (int) STRING_ENCAPSULATOR );
    if( end_string == NULL ) return -1;

    *substr_length = end_string - (begin_string + 1);
    
    substring = (char*) malloc( (*substr_length) + 1 );
    if( substring == NULL ) return -1;

    strncpy(substring, begin_string + 1, (*substr_length));
    substring[(*substr_length) + 1] = '\0';

    return 0;
}

int parse_item( JSONEntry* json_item, char* begin_string ) {
    char* next = begin_string;
    while( (*next) == ' ' ) next++;

    if( next[0] != '"') return -1;

    if( parse_string(json_item->name, NULL, next, next ) == -1 ) return -1;

    while( (*next) == ' ' ) next++;

    if( next[0] != '"') return -1;
}

/** int parse_array ( void* array_ptr, char* string_to_parse, size_t string_to_parse_length ) **//**
 * @brief Parses the array in a string.
 * 
 * @param array_ptr pointer to the result array
 * @param string_to_parse string to parse, pointing to the open array encapsulator char
 * @param string_to_parse_length string to parse length
 * 
 * @return int length of the array
 * @return -1 if error ocurred
 *//***********************************************************************************************/
int parse_array( void* array_ptr, char* string_to_parse, size_t string_to_parse_length ) {
    // Get the array length - number of objects in array
    int array_length = get_array_length(string_to_parse, string_to_parse_length);
    if( array_length == -1 ) return -1;

    // Get the array object type
    JSONType array_object_type = getType(string_to_parse + 1, string_to_parse_length - 1 );

    size_t object_size;
    switch(array_object_type) {
        case JsonObject:
            object_size = sizeof(JSONObject);
            break;
        case String:
            object_size = sizeof(char*);
            break;
        case Integer:
            object_size = sizeof(int);
            break;
        case Float:
            object_size = sizeof(double);
            break;
        case Boolean:
            object_size = sizeof(int);
            break;
        case Array:
            object_size = sizeof(void*);
            break;
        case Null:
            object_size = sizeof(void*);
            break;
        default:
            return -1;
    }

    // Allocate array memory
    array_ptr = malloc( object_size * array_length );
    if (array_ptr == NULL ) return -1;

    for( int i = 0; i < array_length; i++ ) {
        if( next == NULL) return -1;

        ((char*)array_ptr)[ object_size * i ] = parse(next, next - string_to_parse, next);
    }

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