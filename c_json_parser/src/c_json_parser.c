#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../headers/c_json_parser.h"


static int parse(JSONValue* object, char* string_to_parse, char** end_string);

int parse_json(JSONRoot* root, char* string_to_parse)
{
    return parse((JSONValue*) root, string_to_parse, NULL);
}

JSONType get_type(const JSONValue* value)
{
    return value->type;
}

const void* get_value(const JSONValue* value)
{
    return &value->value;
}

const JSONValue* get_key_value(const JSONValue* json_object, char* field_name, void* error)
{
    if( json_object->type != JsonObject ) return NULL;

    for( int i = 0; i < json_object->value.json_object.entries_length; i++ )
    {
        if (strcmp(json_object->value.json_object.entries[i].name, field_name) == 0 )
            return &json_object->value.json_object.entries[i].value;
    }

    return NULL;
}

int* get_integer(const JSONValue* json_object, const char* field_name, void* error)
{
    JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Integer) return NULL;

    return &value->value.integer_value;
}

float* get_float(const JSONValue* json_object, const char* field_name, void* error)
{
    JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Float ) return NULL;

    return &value->value.float_value;
}

char* get_boolean(const JSONValue* json_object, const char* field_name, void* error)
{
    JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Boolean) return NULL;

    return &value->value.boolean_value;
}

char* get_string( const JSONValue* json_object, const char* field_name, void* error)
{
    JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != String ) return NULL;

    return &value->value.string_value;
}

JSONObject* get_json_object( const JSONValue* json_object, const char* field_name, void *error)
{
    JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != JsonObject) return NULL;

    return &value->value.json_object;
}

JSONArray* get_array(const JSONValue* json_object, const char *field_name, void *error)
{
    JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Array ) return NULL;

    return &value->value.array;
}

/******** Private functions ***********/

#ifndef ARRAY_BUFFER_LENGTH
#define ARRAY_BUFFER_LENGTH 10
#endif

#ifndef JOBJECT_BUFFER_LENGTH
#define JOBJECT_BUFFER_LENGTH 10
#endif

#define SHRINK_TO_FIT // If defined, all array and json object will reallocate the objects to fit.

#define ARRAY_OPEN_ENCAPSULATOR '['
#define STRING_ENCAPSULATOR '"'
#define ARRAY_CLOSE_ENCAPSULATOR ']'

static int remove_escaped_chars(char* string) {

    while( (string = strstr(string, "\\\"")) != NULL ) 
    {
        strcpy(string, string + 1);
    }

    return 0;
}

/********************* parse_string *******************//**
 * @brief parses a substring.
 * @details This functions recieves a pointer to a string 
 * enclosed in " chars like so: "<string to parse>".
 * 
 * @pre string_to_parse must be pointing to a " character.
 * @post string enclosed with null char appended and pointer to the end of the string.
 * 
 * @param substring substring extracted.
 * @param substr_length length of the substring. If NULL not recorded.
 * @param begin_string pointer to a " character that contains a string.
 * @param end_string pointer to the last " character.
 * 
 * @return 0 success
 * @return -1 error has occured
*//********************************************************/
static int parse_string( char** substring, int* substr_length, char* begin_string, char** end_string ) {
    // Pointer passed must be pointing to a STRING_ENCAPSULATOR character
    if( (*begin_string) != STRING_ENCAPSULATOR ) return -1;

    // Find end of string
    char* _end_string = begin_string;
    do
    {
        _end_string = strchr(_end_string + 1, (int) STRING_ENCAPSULATOR );
        if( _end_string == NULL ) return -1;

    } while(*( _end_string - 1 ) == '\\');

    // Get the string length
    int substr_length_int = _end_string - (begin_string + 1);

    // Allocate memory
    *substring = (char*) malloc( substr_length_int + 1 );
    if( (*substring) == NULL ) return -1;

    // Copy string and append null character
    strncpy(*substring, begin_string + 1, substr_length_int);
    (*substring)[substr_length_int + 1] = '\0';

    remove_escaped_chars(*substring);

    // return values
    if( substr_length != NULL ) 
        *substr_length = substr_length_int;

    if( end_string != NULL )
        *end_string = _end_string;

    return 0;
}

static int parse_item( JSONEntry* json_item, char* begin_item, char** end_item ) {
    char* next = begin_item;
    while( isspace(*next) ) next++;
    if( *next != '"') return -1;

    if( parse_string(&(json_item->name), NULL, next, &next ) == -1 ) return -1;

    while( isspace(*next) ) next++;
    if( *next != ':' ) return -1;

    if( parse(&(json_item->value), next+1, &next) == -1) return -1;
    next++;
    while( isspace(*next) ) next++;

    if( end_item != NULL) 
        *end_item = next;
    
    return 0;
}

static int parse_object( JSONObject* object, char* begin_item, char** end_item ) {
    if( *begin_item != '{') return -1;

    object->entries_length = 0;
    int max_length = JOBJECT_BUFFER_LENGTH;
    object->entries = (JSONEntry*) malloc(sizeof(JSONEntry) * max_length);
    
    if( object->entries == NULL ) return -1;

    char* next = begin_item;
    while( isspace(*next) ) next++;
    while( *next != '}' && *next != '\0' ) 
    {
        if( object->entries_length >= max_length ) 
        {
            max_length *= 2;
            JSONEntry* new_ptr = realloc(object->entries, max_length);
            if( new_ptr == NULL ) {
                //free_json_object(object);
                return -1;
            }

            object->entries = new_ptr;
        }

        if (parse_item( &object->entries[object->entries_length] , next, &next ) == -1 ) return -1;
        object->entries_length++;
    }

    if( *next != '}' ) return -1;

    #ifdef SHRINK_TO_FIT
    {
        JSONEntry* new_ptr = (JSONEntry*) realloc (object->entries, object->entries_length);
        if( new_ptr == NULL ) {
            //free_json_object(object);
            return -1;
        }

        object->entries = new_ptr;
    }
    #endif

    if( end_item != NULL )
        *end_item = next;

    return 0;
}

/** int parse_array ( void* array_ptr, char* string_to_parse, size_t string_to_parse_length ) **//**
 * @brief Parses the array in a string.
 * 
 * @param array_ptr pointer to the result array
 * @param string_to_parse string to parse, pointing to the open array encapsulator char '['
 * @param string_to_parse_length string to parse length
 * 
 * @return int length of the array
 * @return -1 if error ocurred
 *//***********************************************************************************************/
static int parse_array( JSONArray* array, char* string_to_parse, char** end_string ) {
    if( (*string_to_parse) != '[') return -1;

    size_t array_max_length = ARRAY_BUFFER_LENGTH;
    size_t array_length;

    JSONValue* _array = (JSONValue*) malloc(sizeof(JSONValue) * array_max_length);

    char* next = string_to_parse + 1;
    while( isspace(*next) ) next++;

    for( array_length = 0; (*next) != '\0' && (*next) != ']'; array_length++ ) {
        if( array_length >= array_max_length ) {
            array_max_length *= 2;
            void* new_ptr = realloc(_array, array_max_length);

            if( new_ptr == NULL )
                return -1;

            _array = new_ptr;
        }

        if( (*next) == ',' ) next++;

        if( parse(&_array[array_length], next, &next) == -1 ) return -1;

        next++;
        while( isspace(*next) ) next++;
    }

    array->body = _array;
    array->length = array_length;

    if( end_string != NULL )
        *end_string = next;

    return 0;
}

static int parse_number(JSONValue* object, char* begin_string, char** end_string) 
{
    if( !isdigit(*begin_string) ) return -1;

    object->type = Integer;

    for( int i = 0; isdigit(begin_string[i]) || begin_string[i] == '.'; i++ )
        if( begin_string[i] == '.' )
            object->type = Float;

    if( object->type == Float )
        object->value.float_value = strtod(begin_string, NULL);
    
    else if( object->type == Integer )
        object->value.integer_value = strtoll(begin_string, NULL, 10);

    else
        return -1;

    return 0;
}

static int parse_boolean(int* object, char *begin_string, char **end_string) 
{
    if( !strncmp(begin_string, "true", 4)) 
    {
        *object = 1;
        if( end_string != NULL )
            *end_string = begin_string + 3;
        return 0;
    }
    
    if( !strncmp(begin_string, "false", 5))
    {
        *object = 0;

        if( end_string != NULL )
            *end_string = begin_string + 4;

        return 0;
    }

    return -1;
}

static int parse_null(int* object, char* begin_string, char** end_string) 
{
    if( strncmp(begin_string, "null", 4) )
        return -1;

    *object = 1;

    if( end_string != NULL )
        *end_string = begin_string + 3;

    return 0;
}

static int parse(JSONValue* object, char* string_to_parse, char** end_string) {
    while( isspace(*string_to_parse) ) string_to_parse++;
    char c = (*string_to_parse);

    if( c == '"')
    {
        object->type = String;
        return parse_string(&object->value.string_value, NULL, string_to_parse, end_string);
    }
        
    else if( c == '[') 
    {
        object->type = Array;
        return parse_array(&object->value.array, string_to_parse, end_string);
    }

    else if ( c== '{') 
    {
        object->type = JsonObject;
        return parse_object(&object->value.json_object, string_to_parse, end_string);
    }
        
    else if( isdigit(c) )
        return parse_number(object, string_to_parse, end_string);

    else if( c == 'f' || c == 't')
    {
        object->type = Boolean;
        return parse_boolean(&object->value.boolean_value, string_to_parse, end_string);
    }
    
    else if ( c == 'n') 
    {
        object->type = Null;
        return parse_null(&object->value.is_null, string_to_parse, end_string);
    }

    return -1;
}