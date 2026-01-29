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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of cJSON API calls.
    // It creates a JSON structure, serializes it, minifies the string,
    // parses it back, inspects and manipulates the tree (duplicate, replace,
    // detach/delete), and finally cleans up. Returns 66 on success, negative
    // values on various failure points.

    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic types
    cJSON_AddStringToObject(root, "name", "cJSON");
    cJSON_AddNumberToObject(root, "version", 1.7);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "misc", cJSON_CreateNull());

    // Create an array and add mixed items
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateFalse());
    // Attach the array to the root object
    cJSON_AddItemToObject(root, "items", arr);

    // Create a nested object
    cJSON *child = cJSON_CreateObject();
    if (!child) { cJSON_Delete(root); return -3; }
    cJSON_AddStringToObject(child, "nested_key", "nested_value");
    cJSON_AddItemToObject(root, "child", child);

    // Print formatted JSON (allocated string)
    char *printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return -4; }

    // Make a mutable copy of the printed string and minify it in-place
    size_t len = strlen(printed);
    char *minify_buf = (char*)malloc(len + 1);
    if (!minify_buf) { free(printed); cJSON_Delete(root); return -5; }
    memcpy(minify_buf, printed, len + 1);
    cJSON_Minify(minify_buf);

    // Parse back the minified JSON
    cJSON *parsed = cJSON_Parse(minify_buf);
    free(minify_buf);
    if (!parsed) { free(printed); cJSON_Delete(root); return -6; }

    // Access an item and do a basic validation (value string present)
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!name_item || !name_item->valuestring) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -7;
    }

    // Duplicate the parsed tree (deep duplicate)
    cJSON *dupe = cJSON_Duplicate(parsed, /*recurse=*/1);
    if (!dupe) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -8;
    }

    // Replace the "version" field in the duplicate
    cJSON_ReplaceItemInObject(dupe, "version", cJSON_CreateNumber(2.0));

    // Inspect the "items" array
    cJSON *items = cJSON_GetObjectItemCaseSensitive(dupe, "items");
    if (items) {
        int sz = cJSON_GetArraySize(items);
        // Detach the first array element and delete it separately
        if (sz > 0) {
            cJSON *first = cJSON_DetachItemFromArray(items, 0);
            if (first) {
                // do something with first (here we just delete it)
                cJSON_Delete(first);
            }
        }
        // Replace the second item (now at index 1 originally) with a new string, if present
        if (cJSON_GetArraySize(items) > 1) {
            cJSON_ReplaceItemInArray(items, 1, cJSON_CreateString("replaced"));
        }
    }

    // Remove "misc" from the duplicate object
    cJSON_DeleteItemFromObject(dupe, "misc");

    // Detach "child" from the duplicate, operate on it, then delete it
    cJSON *detached_child = cJSON_DetachItemFromObject(dupe, "child");
    if (detached_child) {
        // potentially manipulate detached_child here...
        cJSON_Delete(detached_child);
    }

    // Clean up all allocated objects and strings
    cJSON_Delete(dupe);
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(root);

    // If we reached here, all operations succeeded
    return 66;
}