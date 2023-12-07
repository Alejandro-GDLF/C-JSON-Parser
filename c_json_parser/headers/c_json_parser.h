/**
 * @file c_json_parser.h
 * @author Alejandro-GDLF@github.com
 * @brief Public header for c json parser public functions.
 * @version 0.1
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _C_JSON_PARSER_H_
#define _C_JSON_PARSER_H_

#include "c_json_parser_types.h"

/// @brief JSON root node
typedef JSONValue JSONRoot;

// Functions
/**
 * @brief Parses a JSON string.
 * 
 * @param[out] root JSON object from the string.
 * @param[in] string_to_parse String to parse.
 * @return -1 if error occurred
 * @return 0 if successful
 */
int parse_json(JSONRoot* root, char* string_to_parse);

/**
 * @brief Get the type of the JSON value object
 * 
 * @param[in] value JSON value which type is retrieved.
 * @return JSON type of the value passed.
 */
JSONType get_type( const JSONValue* value);

/**
 * @brief Get the value of the JSON object.
 * 
 * @param[in] value JSON value which value is retrieved.
 * @return Pointer to the value.
 */
const void* get_value(const JSONValue* value);

/**
 * @brief Get the value from an object given its key.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the value if success.
 * @return NULL if failure.
 */
const JSONValue* get_key_value(const JSONValue* json_object, const char* field_name, void* error);

/******************************************************************************/

// Specific functions
/**
 * @brief Get the integer value of a field.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the integer value if success.
 * @return NULL if failure.
 */
const long long* get_integer(const JSONValue* json_object, const char* field_name, void* error);

/**
 * @brief Get the float value of a field.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the float value if success.
 * @return NULL if failure.
 */
const double* get_float(const JSONValue* json_object, const char* field_name, void* error);

/**
 * @brief Get the boolean value of a field.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the boolean value if success.
 * @return NULL if failure.
 */
const int* get_boolean(const JSONValue* json_object, const char* field_name, void* error);

/**
 * @brief Get the string value of a field.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the string value if success.
 * @return NULL if failure.
 */
const char* get_string( const JSONValue* json_object, const char* field_name, void* error);

/**
 * @brief Get the JSON object of a field.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the JSON object if success.
 * @return NULL if failure.
 */
const JSONObject* get_json_object( const JSONValue* json_object, const char* field_name, void *error);

/**
 * @brief Get the array of a field.
 * 
 * @param json_object JSON object to consult.
 * @param field_name JSON key string.
 * @param error Error produced while consulting.
 * 
 * @return Pointer to the integer value if success.
 * @return NULL if failure.
 */
const JSONArray* get_array(const JSONValue* json_object, const char *field_name, void *error);

// Freeing objects
/**
 * @brief Frees JSON object.
 * 
 * @param[in] root JSON object to free.
 */
void c_json_free (JSONValue *root);

// Printing objects
/**
 * @brief Prints a JSON.
 * 
 * @param[in] root JSON object to print.
*/
void c_json_print(JSONValue *root);

#endif