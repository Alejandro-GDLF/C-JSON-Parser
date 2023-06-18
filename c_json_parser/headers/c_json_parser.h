#ifndef _C_JSON_PARSER_H_
#define _C_JSON_PARSER_H_

#include "c_json_parser_types.h"

/// @brief JSON root node
typedef JSONValue JSONRoot;

// Functions
int parse_json(JSONRoot* root, char* string_to_parse);

JSONType get_type( const JSONValue* value);
const void* get_value(const JSONValue* value);

const JSONValue* get_key_value(const JSONValue* json_object, char* field_name, void* error);

// Specific functions
int* get_integer(const JSONValue* json_object, const char* field_name, void* error);
float* get_float(const JSONValue* json_object, const char* field_name, void* error);
char* get_boolean(const JSONValue* json_object, const char* field_name, void* error);
char* get_string( const JSONValue* json_object, const char* field_name, void* error);
JSONObject* get_json_object( const JSONValue* json_object, const char* field_name, void *error);
JSONArray* get_array(const JSONValue* json_object, const char *field_name, void *error);

#endif