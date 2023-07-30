/**
 * @file c_json_parser_types.h
 * @author Alejandro-GDLF@github.com
 * @brief Header file to describe JSON Parser types.
 * @version 0.1
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _C_JSON_PARSER_TYPES_H_
#define _C_JSON_PARSER_TYPES_H_

#include <stdlib.h>

enum _JSONType;
struct _JSONArray;
struct _JSONObject;
struct _JSONValue;
struct _JSONEntry;

typedef struct _JSONObject  JSONObject;
typedef struct _JSONEntry   JSONEntry;
typedef struct _JSONArray   JSONArray;
typedef struct _JSONValue   JSONValue;
typedef enum   _JSONType    JSONType;

/**
 * @brief Enumeration for identifying the JSON value type.
 * 
 */
enum _JSONType
{
    JsonObject,
    String,
    Integer,
    Float,
    Boolean,
    Array,
    Null
};

/**
 * @brief Array of JSON values.
 * 
 */
struct _JSONArray 
{
    size_t              length;
    JSONValue           *body;
};

/**
 * @brief Array of entries.
 * 
 */
struct _JSONObject
{
    size_t              entries_length;
    JSONEntry           *entries;
};

/**
 * @brief Union of the different types a JSON object can be.
 * 
 */
union _json_values {
    JSONArray           array;
    double              float_value;
    long long           integer_value;
    JSONObject          json_object;
    char                *string_value;
    int                 boolean_value;
    int                 is_null;
};

/**
 * @brief Value and type of the object.
 * 
 */
struct _JSONValue 
{
    JSONType            type;
    union _json_values  value;
};

/**
 * @brief Pair of string and value for JSON objects.
 * 
 */
struct _JSONEntry
{
    char                *name;
    JSONValue           value;
};

#endif