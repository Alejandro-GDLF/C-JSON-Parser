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
    JSON_OBJECT,
    JSON_STRING,
    JSON_INTEGER,
    JSON_FLOAT,
    JSON_BOOLEAN,
    JSON_ARRAY,
    JSON_NULL
};

/**
 * @brief Array of JSON values.
 * 
 */
struct JSONArray 
{
    JSONValue           *body;
    size_t              length;
} __attribute__((packed));

/**
 * @brief Array of entries.
 * 
 */
struct JSONObject
{
    JSONEntry           *entries;
    size_t              entries_length;
};

/**
 * @brief Value and type of the object.
 * 
 */
struct JSONValue 
{
    union {
        JSONArray   array;
        double      float_value;
        long long   integer_value;
        JSONObject  json_object;
        char        *string_value;
        int         boolean_value;
        int         is_null;
    };
    JSONType            type;
};

/**
 * @brief Pair of string and value for JSON objects.
 * 
 */
struct JSONEntry
{
    JSONValue           value;
    char                *name;
};

#endif