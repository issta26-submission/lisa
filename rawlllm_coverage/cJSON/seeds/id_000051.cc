#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add various types to the root object
    cJSON_AddStringToObject(root, "name", "test");
    cJSON_AddNumberToObject(root, "version", 1);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nullable", cJSON_CreateNull());

    // Create an array and add mixed-type elements
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "items", arr);

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return -3; }
    cJSON_AddStringToObject(nested, "inner", "value");
    cJSON_AddItemToObject(root, "nested", nested);

    // Duplicate the root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) { cJSON_Delete(root); return -4; }

    // Replace an item in the duplicate
    cJSON_ReplaceItemInObject(dup, "name", cJSON_CreateString("dupname"));

    // Detach an item from the duplicate, inspect then delete it
    cJSON *detached = cJSON_DetachItemFromObject(dup, "version");
    if (detached) {
        if (!cJSON_IsNumber(detached)) {
            cJSON_Delete(detached);
            cJSON_Delete(dup);
            cJSON_Delete(root);
            return -5;
        }
        cJSON_Delete(detached);
    }

    // Print the duplicate to an unformatted JSON string
    char *printed = cJSON_PrintUnformatted(dup);
    if (!printed) { cJSON_Delete(dup); cJSON_Delete(root); return -6; }

    // Parse the printed string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        free(printed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -7;
    }

    // Validate some parsed contents
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!cJSON_IsArray(items) || cJSON_GetArraySize(items) != 3) {
        free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -8;
    }

    cJSON *nested_parsed = cJSON_GetObjectItemCaseSensitive(parsed, "nested");
    cJSON *inner = nested_parsed ? cJSON_GetObjectItemCaseSensitive(nested_parsed, "inner") : NULL;
    if (!inner || !cJSON_IsString(inner) || strcmp(inner->valuestring, "value") != 0) {
        free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -9;
    }

    // Clean up
    free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // Indicate success as requested
    return 66;
}