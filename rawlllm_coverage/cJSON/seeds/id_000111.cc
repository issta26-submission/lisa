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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object and populate with various types
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    cJSON_AddStringToObject(root, "name", "cJSON test");
    cJSON_AddNumberToObject(root, "version", 1.7);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull());

    // Create an array and add mixed items (numbers, string, and an object)
    cJSON *array = cJSON_CreateArray();
    if (!array) { cJSON_Delete(root); return 0; }

    cJSON_AddItemToArray(array, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(array, cJSON_CreateString("three"));

    cJSON *inner = cJSON_CreateObject();
    if (!inner) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(inner, "inner", "value");
    cJSON_AddItemToArray(array, inner);

    cJSON_AddItemToObject(root, "items", array);

    // Duplicate the whole root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) { cJSON_Delete(root); return 0; }

    // Print the original root to an unformatted JSON string and parse it back
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); cJSON_Delete(dup); return 0; }

    cJSON *parsed = cJSON_Parse(printed);
    // Use cJSON_free to release the string allocated by cJSON_PrintUnformatted
    if (printed) cJSON_free(printed);
    if (!parsed) { cJSON_Delete(root); cJSON_Delete(dup); return 0; }

    // Access an item from the parsed JSON and verify basic properties
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!(name && cJSON_IsString(name) && name->valuestring)) {
        cJSON_Delete(root); cJSON_Delete(dup); cJSON_Delete(parsed); return 0;
    }

    // Detach the "items" array from the parsed document (ownership transferred to caller)
    cJSON *detached_items = cJSON_DetachItemFromObject(parsed, "items");
    // Replace the "version" field in the parsed document
    cJSON_ReplaceItemInObject(parsed, "version", cJSON_CreateNumber(2.0));

    // Move the detached items into the duplicated document under a new key
    if (detached_items) {
        cJSON_AddItemToObject(dup, "detached_items", detached_items);
    }

    // Inspect the moved array inside dup
    cJSON *moved = cJSON_GetObjectItemCaseSensitive(dup, "detached_items");
    if (!moved || !cJSON_IsArray(moved)) {
        cJSON_Delete(root); cJSON_Delete(dup); cJSON_Delete(parsed); return 0;
    }
    int arr_size = cJSON_GetArraySize(moved);
    if (arr_size < 3) { // we originally added at least 3 elements
        cJSON_Delete(root); cJSON_Delete(dup); cJSON_Delete(parsed); return 0;
    }

    // Duplicate a single array element and add it back into the original root's array
    cJSON *third = cJSON_GetArrayItem(moved, 2);
    if (!third) { cJSON_Delete(root); cJSON_Delete(dup); cJSON_Delete(parsed); return 0; }
    cJSON *third_dup = cJSON_Duplicate(third, 1);
    if (!third_dup) { cJSON_Delete(root); cJSON_Delete(dup); cJSON_Delete(parsed); return 0; }

    cJSON *orig_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    if (!orig_items || !cJSON_IsArray(orig_items)) {
        cJSON_Delete(third_dup);
        cJSON_Delete(root); cJSON_Delete(dup); cJSON_Delete(parsed); return 0;
    }
    cJSON_AddItemToArray(orig_items, third_dup);

    // Optionally produce a pretty-printed string of dup to exercise printing API
    char *dup_print = cJSON_Print(dup);
    if (dup_print) cJSON_free(dup_print);

    // Clean up: delete the JSON trees we own
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);

    // If we reached here everything that we created/parsed/duplicated was handled correctly
    return 66;
}