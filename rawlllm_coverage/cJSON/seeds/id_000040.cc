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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API usages:
    // - create objects/arrays/primitives
    // - add/replace/detach items
    // - print and parse JSON text
    // - duplicate and remove items
    // - proper cleanup
    //
    // It returns 66 on success, and a non-66 value on failure.

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic items
    cJSON_AddStringToObject(root, "name", "Test Name");
    cJSON_AddNumberToObject(root, "value", 42);
    // CreateBool is available in modern cJSON; if your cJSON doesn't have it,
    // use cJSON_CreateTrue()/cJSON_CreateFalse() or cJSON_AddBoolToObject variant.
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateBool(1));

    // Create an array and populate it
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateBool(0));
    cJSON_AddItemToObject(root, "items", arr);

    // Print unformatted JSON text
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return -3; }

    // Parse the generated JSON text back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    // printed was allocated by cJSON_PrintUnformatted (malloc); free it
    free(printed);
    if (!parsed) { cJSON_Delete(root); return -4; }

    // Validate a value from the parsed tree
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL) || strcmp(name->valuestring, "Test Name") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -5;
    }

    // Replace a numeric item in the original root
    // (cJSON_ReplaceItemInObject returns void in typical cJSON API)
    cJSON_ReplaceItemInObject(root, "value", cJSON_CreateNumber(100));

    // Detach (remove and return) an item and re-add it under a new key
    cJSON *detached = cJSON_DetachItemFromObject(root, "name");
    if (!detached) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -6;
    }
    cJSON_AddItemToObject(root, "renamed", detached);

    // Duplicate the modified root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1); // recurse = 1 for deep copy
    if (!dup) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -7;
    }
    // We don't need dup further; delete it
    cJSON_Delete(dup);

    // Get the items array and remove the second element (index 1)
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    if (!cJSON_IsArray(items)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -8;
    }
    // Typical cJSON function: cJSON_DeleteItemFromArray(array, which);
    cJSON_DeleteItemFromArray(items, 1);

    // Cleanup parsed and root structures
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Success
    return 66;
}