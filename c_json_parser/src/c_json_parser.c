/**
 * @file c_json_parser.c
 * @author Alejandro-GDLF@github.com
 * @brief Source code file for c_json_parser
 * @version 0.1
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../headers/c_json_parser.h"

// ******************* Static functions ***************************************
static int parse(JSONValue*, char*, char**);
static int parse_string( char**, char*, char**);
static int parse_item( JSONEntry*, char*, char**);
static int parse_object( JSONObject*, char*, char**);
static int parse_array( JSONArray*, char*, char**);
static int parse_number(JSONValue*, char*, char**);
static int parse_boolean(int*, char*, char **);
static int parse_null(int*, char*, char**);
static void free_j_object(JSONObject*);
static void free_j_array(JSONArray*);

//******************************************************************************

int parse_json(JSONRoot* root, char* string_to_parse)
{
    if( parse((JSONValue*) root, string_to_parse, NULL) == -1)
    {
        c_json_free(root);
        return -1;
    }
    
    return 0;
}

JSONType get_type(const JSONValue* value)
{
    return value->type;
}

const void* get_value(const JSONValue* value)
{
    return &value->value;
}

const JSONValue* get_key_value(const JSONValue* json_object, const char* field_name, void* error)
{
    if( json_object->type != JsonObject ) return NULL;

    for( size_t i = 0; i < json_object->value.json_object.entries_length; i++ )
    {
        if (strcmp(json_object->value.json_object.entries[i].name, field_name) == 0 )
            return &json_object->value.json_object.entries[i].value;
    }

    return NULL;
}

const long long* get_integer(const JSONValue* json_object, const char* field_name, void* error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Integer) return NULL;

    return &value->value.integer_value;
}

const double* get_float(const JSONValue* json_object, const char* field_name, void* error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Float ) return NULL;

    return &value->value.float_value;
}

const int* get_boolean(const JSONValue* json_object, const char* field_name, void* error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Boolean) return NULL;

    return &value->value.boolean_value;
}

const char* get_string( const JSONValue* json_object, const char* field_name, void* error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != String ) return NULL;

    return (const char*)&value->value.string_value;
}

const JSONObject* get_json_object( const JSONValue* json_object, const char* field_name, void *error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != JsonObject) return NULL;

    return &value->value.json_object;
}

const JSONArray* get_array(const JSONValue* json_object, const char *field_name, void *error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Array ) return NULL;

    return &value->value.array;
}

void c_json_free (JSONValue *value)
{
    if( value->type == JsonObject )
        free_j_object(&value->value.json_object);

    else if( value->type == Array )
        free_j_array(&value->value.array);

    else if (value->type == String )
        free((void*)value->value.string_value);
}

//************************** Static functions *********************************

#ifndef ARRAY_BUFFER_LENGTH
#define ARRAY_BUFFER_LENGTH 10
#endif

#ifndef JOBJECT_BUFFER_LENGTH
#define JOBJECT_BUFFER_LENGTH 10
#endif

// If defined, all array and json object will reallocate the objects to fit.
#define SHRINK_TO_FIT

#define ARRAY_OPEN_ENCAPSULATOR '['
#define STRING_ENCAPSULATOR '"'
#define ARRAY_CLOSE_ENCAPSULATOR ']'

static int parse(JSONValue* object, char* string_to_parse, char** end_string) 
{
    if( object == NULL ) return -1;
    while( isspace(*string_to_parse) ) string_to_parse++;

    char c = (*string_to_parse);

    if( c == '"')
    {
        object->type = String;
        return parse_string(&object->value.string_value, string_to_parse, end_string);
    }
        
    else if( c == '[') 
    {
        object->type = Array;
        return parse_array(&object->value.array, string_to_parse, end_string);
    }

    else if ( c == '{') 
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

static int remove_escaped_chars(char* string) 
{
    while( (string = strstr(string, "\\\"")) != NULL )
        strcpy(string, string + 1);

    return 0;
}

/********************* parse_string ****************************************//**
 * @brief parses a substring encolsed in '"' characters.
 * @details This functions recieves a pointer to a string 
 * enclosed in " chars like so: "<string to parse>".
 * 
 * @pre string_to_parse must be pointing to a " character.
 * @post string enclosed with null char appended and pointer to the 
 * end of the string + 1.
 * 
 * @param[out] substring substring extracted.
 * @param[in]  begin_string pointer to a " character that contains a string.
 * @param[out] end_string pointer to the first char after string representation.
 * 
 * @return 0 success
 * @return -1 error has occured
*//****************************************************************************/
static int parse_string( char** substring, char* begin_string, char** end_string ) 
{
    // Pointer passed must be pointing to a STRING_ENCAPSULATOR character
    if( begin_string == NULL ) return -1;
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
    if( (*substring) == NULL ) 
        return -1;

    // Copy string and append null character
    strncpy(*substring, begin_string + 1, substr_length_int);
    (*substring)[substr_length_int] = '\0';

    remove_escaped_chars(*substring);

    // return values
    if( end_string != NULL )
        *end_string = _end_string + 1;

    return 0;
}

static int parse_item( JSONEntry* json_item, char* begin_item, char** end_item ) 
{
    if( json_item == NULL ) return -1;

    char* next = begin_item;
    while( isspace(*next) ) next++;

    if( parse_string(&(json_item->name), next, &next ) == -1 ) return -1;

    while( isspace(*next) ) next++;
    if( *next != ':' ) return -1;

    if( parse(&(json_item->value), next+1, &next) == -1) return -1;
    while( isspace(*next) ) next++;

    if( end_item == NULL)
        return 0;

    *end_item = next;

    if( *next == ';' )
        *end_item += 1;
    
    return 0;
}

static int parse_object( JSONObject* object, char* begin_item, char** end_item ) 
{
    if( *begin_item != '{') return -1;  // Not a JSON object
    if( object == NULL ) return -1;     // Null pointer to the type

    char* next = begin_item + 1;
    while( isspace(*next) ) next++;

    // Malformed JSON Object
    if( *next == '\0')
        return -1;

    // Empty JSON Object
    object->entries_length = 0;
    if( *next == '}' )
        goto finish;

    size_t max_length = JOBJECT_BUFFER_LENGTH;
    object->entries = (JSONEntry*) malloc(sizeof(JSONEntry) * max_length);
    if( object->entries == NULL ) 
        return -1;

    while( *next != '}' && *next != '\0' )
    {
        if( object->entries_length >= max_length ) 
        {
            max_length *= 2;
            JSONEntry* new_ptr = realloc(object->entries, max_length);
            if( new_ptr == NULL ) 
                return -1;

            object->entries = new_ptr;
        }

        if (parse_item( &object->entries[object->entries_length] , next, &next ) == -1 ) return -1;
        object->entries_length++;
    }

    if( *next != '}' ) return -1;

    finish:

    #ifdef SHRINK_TO_FIT
    {
        JSONEntry *reallocated_entries = realloc(object->entries, object->entries_length * sizeof(JSONEntry));
        if ( reallocated_entries != NULL )
            object->entries = reallocated_entries;
    }
    #endif

    if( end_item != NULL )
        *end_item = next + 1;

    return 0;
}

/**************************** parse_array **********************************//**
 * @brief Parses the array in a JSON string.
 * 
 * @param[out] array_ptr pointer to the result array.
 * @param[in] string_to_parse string to parse, pointing to the open array 
 * encapsulator.
 * @param[out] end_string end of the array. Pointer to the ']' char + 1.
 * 
 * @return int length of the array
 * @return -1 if error ocurred
 *//***************************************************************************/
static int parse_array( JSONArray* array, char* string_to_parse, char** end_string ) 
{
    if( (*string_to_parse) != '[') return -1;
    if( array == NULL ) return -1;

    size_t array_max_length = ARRAY_BUFFER_LENGTH;
    array->length = 0;

    array->body = (JSONValue*) malloc(sizeof(JSONValue) * array_max_length);

    char* next = string_to_parse + 1;

    while( isspace(*next) ) next++;
    if( *next == ']' ) goto finish;
    next--;

    do
    {
        if( array->length >= array_max_length ) {
            array_max_length *= 2;
            void* new_ptr = realloc(array->body, array_max_length);

            if( new_ptr == NULL )
                return -1;

            array->body = new_ptr;
        }

        next++;
        while( isspace(*next) ) next++;
        if( parse(&array->body[array->length], next, &next) == -1 ) return -1;
        while( isspace(*next) ) next++;

        array->length++;

    } while (*next != '\0' && *next != ']');

    if( *next != ']' ) return -1;

    finish:

    if( end_string != NULL )
        *end_string = next + 1;

    return 0;
}

static int parse_number(JSONValue* object, char* begin_string, char** end_string) 
{
    if( !isdigit(*begin_string) ) return -1;

    object->type = Integer;

    size_t i;
    for( i = 0; isdigit(begin_string[i]) || begin_string[i] == '.'; i++ )
        if( begin_string[i] == '.' )
            object->type = Float;

    if( object->type == Float )
        object->value.float_value = strtod(begin_string, NULL);
    
    else if( object->type == Integer )
        object->value.integer_value = strtoll(begin_string, NULL, 10);

    else
        return -1;
    
    if( end_string != NULL )
        *end_string = begin_string + i;

    return 0;
}

static int parse_boolean(int* object, char *begin_string, char **end_string) 
{
    if( strncmp(begin_string, "true", 4) == 0) 
    {
        *object = 1;
        if( end_string != NULL )
            *end_string = begin_string + 4;
        return 0;
    }
    
    if( strncmp(begin_string, "false", 5) == 0)
    {
        *object = 0;

        if( end_string != NULL )
            *end_string = begin_string + 5;

        return 0;
    }

    return -1;
}

static int parse_null(int* object, char* begin_string, char** end_string)
{
    if( strncmp(begin_string, "null", 4) != 0 )
        return -1;

    *object = 1;

    if( end_string != NULL )
        *end_string = begin_string + 4;

    return 0;
}

static void free_j_object(JSONObject *object)
{
    for( size_t i = 0; i < object->entries_length; i++)
    {
        free(object->entries[i].name);
        c_json_free(&object->entries[i].value);
    }

    free(object->entries);
}

static void free_j_array(JSONArray *array)
{
    for(size_t i  = 0; i < array->length; i++ )
        c_json_free(&array->body[i]);

    free(array->body);
}
