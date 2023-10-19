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

#ifdef DEBUG
#define LOG(x, ...) printf(x, ##__VA_ARGS__)
#include <stdio.h>
#else
#define LOG(x, ...)
#endif

#include "c_json_parser.h"

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
static void* _get(const JSONValue*, const char*, JSONType);
static void consume_blank_char(char**);

//******************************************************************************

int parse_json(JSONRoot* root, char* string_to_parse)
{
    if( parse((JSONValue*) root, string_to_parse, NULL) == -1)
    {
        c_json_free(root);
        LOG("Parse error\n");
        return -1;
    }
    
    return 0;
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
    return (const long long*)_get(json_object, field_name, Integer);
}

const double* get_float(const JSONValue* json_object, const char* field_name, void* error)
{
    return (const double*)_get(json_object, field_name, Float);
}

const int* get_boolean(const JSONValue* json_object, const char* field_name, void* error)
{
    return (const int*)_get(json_object, field_name, Boolean);
}

const char* get_string( const JSONValue* json_object, const char* field_name, void* error)
{
    return (const char*)_get(json_object, field_name, String);
}

const JSONObject* get_json_object( const JSONValue* json_object, const char* field_name, void *error)
{
    return (const JSONObject*)_get(json_object, field_name, JsonObject);
}

const JSONArray* get_array(const JSONValue* json_object, const char *field_name, void *error)
{
    return (const JSONArray*)_get(json_object, field_name, Array);
}

//************************** Static functions *********************************

#ifndef ARRAY_BUFFER_LENGTH
#define ARRAY_BUFFER_LENGTH 10
#endif

#ifndef JOBJECT_BUFFER_LENGTH
#define JOBJECT_BUFFER_LENGTH 10
#endif

#define JSON_ARRAY_OPEN '['
#define JSON_ARRAY_CLOSE ']'

#define JSON_STRING_OPEN '"'
#define JSON_STRING_CLOSE '"'

#define JSON_OBJECT_OPEN '{'
#define JSON_OBJECT_CLOSE '}'

#define JSON_OBJECT_ENTRY_SPACER ':'
#define JSON_OBJECT_ITEM_SPACER ','

#define JSON_ARRAY_ITEM_SPACER ','

#define NULL_CHAR '\0'
#define BACKSLASH_CHAR '\\'

#define DECIMAL_SEPARATOR '.'

static int parse(JSONValue* object, char* string_to_parse, char** end_string) 
{
    if( object == NULL )
    {
        LOG("NULL object pointer\n");
        return -1;
    }

    consume_blank_char(&string_to_parse);

    char c = (*string_to_parse);

    switch(c) 
    {
        case JSON_STRING_OPEN:
            object->type = String;
            return parse_string(&object->value.string_value, string_to_parse, end_string);

        case JSON_ARRAY_OPEN:
            object->type = Array;
            return parse_array(&object->value.array, string_to_parse, end_string);
        
        case JSON_OBJECT_OPEN:
            object->type = JsonObject;
            return parse_object(&object->value.json_object, string_to_parse, end_string);
        
        case '-':
            return parse_number(object, string_to_parse, end_string);
        
        case 'f':
        case 't':
            object->type = Boolean;
            return parse_boolean(&object->value.boolean_value, string_to_parse, end_string);

        case 'n':
            object->type = Null;
            return parse_null(&object->value.is_null, string_to_parse, end_string);

        default:
            if (isdigit(c))
                return parse_number(object, string_to_parse, end_string);
            
            LOG("Character not supported: %c\n", c);
            return -1;
    }
}

static void* _get(const JSONValue* json_object, const char *field_name, JSONType type)
{
    const JSONValue* value = get_key_value(json_object, field_name, NULL);

    if( value == NULL ) return NULL;
    if( value->type != type) return NULL;

    return (void*) &value->value;
}

static void consume_blank_char(char** ptr)
{
    while(isspace(**ptr))
        (*ptr)++;
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
 * @pre substring must not be NULL
 * @post string enclosed with null char appended and pointer to the 
 * end of the string + 1.
 * 
 * @param[out] substring substring extracted.
 * @param[in]  begin_string pointer to a string wanted to parse.
 * @param[out] end_string pointer to the first char after string representation. Can be NULL
 * 
 * @return 0 success
 * @return -1 error has occured
*//****************************************************************************/
static int parse_string( char** substring, char* begin_string, char** end_string ) 
{
    LOG("Parsing string\n");
    // Pointer passed must be pointing to a STRING_ENCAPSULATOR character
    if( begin_string == NULL ) 
    {
        LOG("String: begin string NULL");
        return -1;
    }

    if( (*begin_string) != JSON_STRING_OPEN )
    {
        LOG("String: No string encapsulator\n");
        return -1;
    }

    // Find end of string
    char* _end_string = begin_string;
    do
    {
        _end_string = strchr(_end_string + 1, (int) JSON_STRING_OPEN );
        if( _end_string == NULL )
        {
            LOG("String: Malformed string\n");
            return -1;
        }
    } while(*( _end_string - 1 ) == '\\'); // Repeat if " char is escaped.

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
    if( json_item == NULL ) {
        LOG("JSON item: NULL JSON item\n");
        return -1;
    }

    char* next = begin_item;
    
    consume_blank_char(&next);

    if( parse_string(&(json_item->name), next, &next ) == -1 ) 
    {
        LOG("JSON item: Can't parse string\n");
        return -1;
    }

    consume_blank_char(&next);

    if( *next != ':' )
    {
        LOG("Malformed JSON item\n");
        return -1;
    }

    if( parse(&(json_item->value), next+1, &next) == -1)
    {
        LOG("JSON item: can't parse JSON value\n");
        return -1;
    }

    consume_blank_char(&next);

    if( end_item == NULL)
        return 0;

    *end_item = next;

    if( *next == JSON_OBJECT_ITEM_SPACER )
        (*end_item) += 1;
    
    return 0;
}

static int parse_object( JSONObject* object, char* begin_item, char** end_item ) 
{
    LOG("Parsing JSON object\n");
    // Not a JSON object
    if( *begin_item != '{') 
    {
        LOG("Not a JSON object\n");
        return -1;
    }

    // Null pointer to the type
    if( object == NULL )
    {
        LOG("NULL object pointer\n");
        return -1;
    }

    char* next = begin_item + 1;
    consume_blank_char(&next);

    // Malformed JSON Object
    if( *next == '\0')
    {
        LOG("Malformed JSON object\n");
        return -1;
    }

    // Empty JSON Object
    object->entries_length = 0;
    if( *next == JSON_OBJECT_CLOSE )
        goto finish;

    size_t max_length = JOBJECT_BUFFER_LENGTH;
    object->entries = (JSONEntry*) malloc(sizeof(JSONEntry) * max_length);
    if( object->entries == NULL ) 
    {
        LOG("Couldn't allocate memory for JSOn Object entries\n");
        return -1;
    }

    while( *next != JSON_OBJECT_CLOSE && *next != NULL_CHAR )
    {
        if( object->entries_length >= max_length ) 
        {
            max_length *= 2;
            JSONEntry* new_ptr = realloc(object->entries, max_length);
            if( new_ptr == NULL ) 
            {
                LOG("Unable to allocate more memory\n");
                return -1;
            }
                
            object->entries = new_ptr;
        }

        if (parse_item( &object->entries[object->entries_length] , next, &next ) == -1 ) return -1;
        object->entries_length++;
    }

    if( *next != JSON_OBJECT_CLOSE )
    {
        LOG("Not final object. Malformed\n");
        free_j_object(object);
        return -1;
    }

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
    if( array == NULL ) return -1;
    if( (*string_to_parse) != JSON_ARRAY_OPEN) return -1;

    size_t array_max_length = ARRAY_BUFFER_LENGTH;
    array->length = 0;

    array->body = (JSONValue*) malloc(sizeof(JSONValue) * array_max_length);

    char* next = string_to_parse + 1;

    consume_blank_char(&next);

    while (*next != NULL_CHAR && *next != JSON_ARRAY_CLOSE)
    {
        if( array->length >= array_max_length ) {
            array_max_length *= 2;
            void* new_ptr = realloc(array->body, array_max_length);

            if( new_ptr == NULL )
                return -1;

            array->body = new_ptr;
        }

        if(*next == JSON_ARRAY_ITEM_SPACER) 
            next++;

        if( parse(&array->body[array->length], next, &next) == -1 ) return -1;
        array->length++;

        consume_blank_char(&next);

    }

    if( *next != JSON_ARRAY_CLOSE ) return -1;

    finish:

    if( end_string != NULL )
        *end_string = next + 1;

    return 0;
}

static int parse_number(JSONValue* object, char* begin_string, char** end_string) 
{
    LOG("Parsing number\n");
    if( !isdigit(*begin_string) && *begin_string != '-' ) return -1;
    
    int modifier = 1;
    if(*begin_string == '-')
    {
        modifier = -1;
        begin_string++;
    }

    object->type = Integer;

    size_t i;
    for( i = 0; isdigit(begin_string[i]) || begin_string[i] == DECIMAL_SEPARATOR; i++ )
        if( begin_string[i] == DECIMAL_SEPARATOR )
            object->type = Float;

    if( object->type == Float )
        object->value.float_value = strtod(begin_string, NULL) * modifier;
    
    else if( object->type == Integer )
        object->value.integer_value = strtoll(begin_string, NULL, 10) * modifier;

    else
        return -1;
    
    if( end_string != NULL )
        *end_string = begin_string + i;

    return 0;
}

static int parse_boolean(int* object, char *begin_string, char **end_string) 
{
    LOG("Parsing boolean\n");
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
    LOG("Parsing null\n");
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
