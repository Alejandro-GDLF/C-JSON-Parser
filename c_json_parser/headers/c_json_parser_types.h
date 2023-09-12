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

enum    JSONType;
struct  JSONArray;
struct  JSONObject;
struct  JSONValue;
struct  JSONEntry;

typedef struct JSONObject  JSONObject;
typedef struct JSONEntry   JSONEntry;
typedef struct JSONArray   JSONArray;
typedef struct JSONValue   JSONValue;
typedef enum   JSONType    JSONType;

/**
 * @brief Enumeration for identifying the JSON value type.
 * 
 */
enum JSONType
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
struct JSONArray 
{
    size_t              length;
    JSONValue           *body;
};

/**
 * @brief Array of entries.
 * 
 */
struct JSONObject
{
    size_t              entries_length;
    JSONEntry           *entries;
};

/**
 * @brief Value and type of the object.
 * 
 */
struct JSONValue 
{
    JSONType            type;
    union {
        JSONArray   array;
        double      float_value;
        long long   integer_value;
        JSONObject  json_object;
        char        *string_value;
        int         boolean_value;
        int         is_null;
    }                   value;
};

/**
 * @brief Pair of string and value for JSON objects.
 * 
 */
struct JSONEntry
{
    char                *name;
    JSONValue           value;
};

#endif