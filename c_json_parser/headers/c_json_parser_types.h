#ifndef _C_JSON_PARSER_TYPES_H_
#define _C_JSON_PARSER_TYPES_H_

enum _JSONType;
struct _JSONArray;
struct _JSONObject;
struct _JSONValue;
struct _JSONEntry;

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
    struct _JSONValue*  body;
};

struct _JSONObject
{
    size_t              entries_length;
    struct _JSONEntry*  entries;
};

union _json_values {
    struct _JSONArray   array;
    double              float_value;
    long long           integer_value;
    struct _JSONObject  json_object;
    char                *string_value;
    int                 boolean_value;
    int                 is_null;
};

struct _JSONValue 
{
    enum _JSONType      type;
    union _json_values  value;
};

struct _JSONEntry
{
    char                *name;
    struct _JSONValue   value;
};

typedef struct _JSONObject  JSONObject;
typedef struct _JSONEntry   JSONEntry;
typedef struct _JSONArray   JSONArray;
typedef struct _JSONValue   JSONValue;
typedef enum   _JSONType    JSONType;

#endif