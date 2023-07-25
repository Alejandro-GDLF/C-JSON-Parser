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

struct _JSONArray 
{
    size_t              length;
    JSONValue           *body;
};

struct _JSONObject
{
    size_t              entries_length;
    JSONEntry           *entries;
};

union _json_values {
    JSONArray           array;
    double              float_value;
    long long           integer_value;
    JSONObject          json_object;
    char                *string_value;
    int                 boolean_value;
    int                 is_null;
};

struct _JSONValue 
{
    JSONType            type;
    union _json_values  value;
};

struct _JSONEntry
{
    char                *name;
    JSONValue           value;
};

#endif