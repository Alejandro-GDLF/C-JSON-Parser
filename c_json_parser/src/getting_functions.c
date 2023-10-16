#include "c_json_parser.h"

static void* _get(const JSONValue* json_object, const char *field_name, JSONType type)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != JsonObject) return NULL;

    return (void*) &value->value;
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
    return (JSONObject*)_get(json_object, field_name, JsonObject);
}

const JSONArray* get_array(const JSONValue* json_object, const char *field_name, void *error)
{
    const JSONValue* value = get_key_value(json_object, field_name, error);

    if( value == NULL ) return NULL;
    if( value->type != Array ) return NULL;

    return &value->value.array;
}