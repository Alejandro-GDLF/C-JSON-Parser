#ifndef _C_JSON_PARSER_H_
#define _C_JSON_PARSER_H_

// Types
typedef enum {
    JSONObject,
    String,
    Integer,
    Float,
    Boolean,
    Array,
    Null
} JSONType;

typedef struct {
    char *name;
    JSONType type;
    void *value;
} JSONEntry;

typedef struct {
    JSONEntry *entries;
    size_t entries_length;
} JSONObject;

typedef JSONEntry* JSONRoot;


// Functions
JSONRoot* parse_string(char* string_to_parse, size_t string_to_parse_length);

void* get_value(JSONObject* json_object, char* field_name, JSONType* value_type, void* error);

// Specified functions
int* get_integer(JSONObject* json_object, char* field_name, void* error);
float* get_float(JSONObject* json_object, char* field_name, void* error);
bool* get_boolean(JSONObject* json_object, char* field_name, void* error);
char* get_string(JSONObject* json_object, char* field_name, void* error);
JSONObject* get_json_object(JSONObject* json_object, char* field_name, void* error);

#endif