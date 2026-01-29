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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Include headers here so this function can be pasted standalone into a file.
    // (Preprocessor directives can appear anywhere in the file; this keeps the
    // function self-contained for the purpose of this test.)
    #include <cstdlib>
    #include <cstring>
    #include "cJSON.h"

    // We'll create a JSON, parse it, inspect items, duplicate nodes,
    // create new nodes/arrays/objects, detach and reattach an item,
    // print/unformatted string, minify a buffer, then clean up.
    const char *raw = "{\"name\":\"John\",\"age\":30,\"items\":[\"a\",2,true],\"nested\":{\"x\":1}}";

    cJSON *root = cJSON_Parse(raw);
    if (!root) {
        return 0; // parse failed
    }

    // Access string item "name"
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!name || !name->valuestring) {
        cJSON_Delete(root);
        return 0;
    }

    // Access array "items" and check its size and second element
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    if (!items || !cJSON_IsArray(items)) {
        cJSON_Delete(root);
        return 0;
    }
    int items_size = cJSON_GetArraySize(items);
    if (items_size < 2) {
        cJSON_Delete(root);
        return 0;
    }
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    if (!second_item) {
        cJSON_Delete(root);
        return 0;
    }

    // Duplicate the original root (deep copy)
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    if (!dup_root) {
        cJSON_Delete(root);
        return 0;
    }

    // Create a new object and populate it using various cJSON creation functions
    cJSON *newObj = cJSON_CreateObject();
    if (!newObj) {
        cJSON_Delete(dup_root);
        cJSON_Delete(root);
        return 0;
    }

    // Add a copied string (from original) and a number
    cJSON_AddStringToObject(newObj, "copied_name", name->valuestring);
    cJSON_AddNumberToObject(newObj, "count", 123);

    // Add a deep-copied "items" array into newObj
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    if (!items_copy) {
        cJSON_Delete(newObj);
        cJSON_Delete(dup_root);
        cJSON_Delete(root);
        return 0;
    }
    cJSON_AddItemToObject(newObj, "items_copy", items_copy);

    // Create an array, add various items, and attach to newObj
    cJSON *arr2 = cJSON_CreateArray();
    if (!arr2) {
        cJSON_Delete(newObj);
        cJSON_Delete(dup_root);
        cJSON_Delete(root);
        return 0;
    }
    cJSON_AddItemToArray(arr2, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr2, cJSON_CreateNumber(42));
    cJSON_AddItemToObject(newObj, "arr2", arr2);

    // Detach "nested" from root and re-add it under a new name
    cJSON *detached_nested = cJSON_DetachItemFromObject(root, "nested");
    if (detached_nested) {
        // Reattach under a different key
        cJSON_AddItemToObject(root, "nested_readded", detached_nested);
        // It's now part of root again and will be freed when root is deleted.
    }

    // Use cJSON_Duplicate on a child (shallow copy)
    cJSON *shallow_items_dup = cJSON_Duplicate(items, 0);
    if (shallow_items_dup) {
        // add shallow duplicate to newObj to demonstrate API
        cJSON_AddItemToObject(newObj, "shallow_items_dup", shallow_items_dup);
    }

    // Remove the first element of arr2 (demonstrate DeleteItemFromArray)
    if (cJSON_GetArraySize(arr2) > 0) {
        cJSON_DeleteItemFromArray(arr2, 0);
    }

    // Print newObj to a compact string
    char *printed = cJSON_PrintUnformatted(newObj);
    if (!printed) {
        // cleanup and fail
        cJSON_Delete(newObj);
        cJSON_Delete(dup_root);
        cJSON_Delete(root);
        return 0;
    }

    // Create a modifiable copy of the raw JSON, then minify it
    size_t raw_len = strlen(raw);
    char *minify_buf = (char*)malloc(raw_len + 1);
    if (!minify_buf) {
        free(printed);
        cJSON_Delete(newObj);
        cJSON_Delete(dup_root);
        cJSON_Delete(root);
        return 0;
    }
    strcpy(minify_buf, raw);
    cJSON_Minify(minify_buf);

    // Basic verification checks: printed must be non-empty and minified buffer shorter or equal
    if (printed[0] == '\0' || strlen(minify_buf) == 0) {
        free(minify_buf);
        free(printed);
        cJSON_Delete(newObj);
        cJSON_Delete(dup_root);
        cJSON_Delete(root);
        return 0;
    }

    // Clean up: newObj, dup_root, and root (root contains any reattached detached_nested)
    free(minify_buf);
    free(printed);
    cJSON_Delete(newObj);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // If all operations succeeded, return 66 as requested
    return 66;
}