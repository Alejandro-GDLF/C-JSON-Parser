#include <stdio.h>
#include <string.h>

#include "../../headers/c_json_parser.h"

int main()
{
    char input[100] = "{\"name\": \"John\", \"age\": 31, \"city\": \"New York\"}";
    char names[3][100] = { "name", "age", "city" };

    JSONRoot root;
    if(parse_json(&root, input) == -1 )
    {
        printf("Test1: parse returned -1\n");
        return -1;
    }

    if( root.type != JsonObject )
    {
        printf("Test1: parse returned wrong type: %d\n", root.type);
        return -1;
    }

    for( int i = 0; i < root.value.json_object.entries_length; i++ )
    {
        if(strcmp(root.value.json_object.entries[i].name, names[i]) != 0 )
        {
            printf("-Rec: %d. -Exp: %d.\n", strlen(root.value.json_object.entries[i].name), strlen(names[i]));
            printf("Names not equal.\n");
            printf("\t- Name: %s\n", root.value.json_object.entries[i].name);
            printf("\t- Expected: %s\n", names[i]);
            return -1;
        }
    }

    if( strcmp(root.value.json_object.entries[0].value.value.string_value, "John") != 0 )
    {
        printf("Values does not match: \n");
        printf("\t-Expected: Jhon\n");
        printf("\t-Recieved: %s.", root.value.json_object.entries[0].value.value.string_value);
        return -1;
    }
    
    if( root.value.json_object.entries[1].value.value.integer_value != 31 )
    {
        printf("Values does not match: \n");
        printf("\t-Expected: 31.\n");
        printf("\t-Recieved: %d.", root.value.json_object.entries[1].value.value.integer_value);
        return -1;
    }

    if( strcmp(root.value.json_object.entries[2].value.value.string_value, "New York") != 0 )
    {
        printf("Values does not match: \n");
        printf("\t-Expected: New York\n");
        printf("\t-Recieved: %s.", root.value.json_object.entries[2].value.value.string_value);
        return -1;
    }

    printf("Test: passed successfully");
}