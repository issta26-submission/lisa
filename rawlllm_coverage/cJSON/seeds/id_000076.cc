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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add basic types
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("cJSON test"));
    cJSON_AddItemToObject(root, "version", cJSON_CreateNumber(1.7));
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull());

    // Create and populate an array, then add to root
    cJSON *numbers = cJSON_CreateArray();
    if (!numbers) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "numbers", numbers);

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(nested, "inner", cJSON_CreateString("value"));
    cJSON_AddItemToObject(root, "nested", nested);

    // Duplicate the whole structure (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1); // recursive = 1
    if (!dup) {
        cJSON_Delete(root);
        return 0;
    }

    // Replace a field in the duplicate
    cJSON_ReplaceItemInObject(dup, "version", cJSON_CreateNumber(2.0));

    // Detach the nested object from the duplicate and reattach under root with a new key
    cJSON *detached = cJSON_DetachItemFromObject(dup, "nested");
    if (detached) {
        cJSON_AddItemToObject(root, "restored_nested", detached);
        // detached is now owned by root; do not delete separately
    }

    // Parse a JSON text to exercise the parser API
    const char *json_text = "{\"a\":1, \"b\":true, \"arr\":[\"x\",\"y\"]}";
    cJSON *parsed = cJSON_Parse(json_text);
    if (!parsed) {
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return 0;
    }

    // Inspect parsed contents
    cJSON *a_item = cJSON_GetObjectItemCaseSensitive(parsed, "a");
    cJSON *b_item = cJSON_GetObjectItemCaseSensitive(parsed, "b");
    cJSON *arr_item = cJSON_GetObjectItemCaseSensitive(parsed, "arr");

    // Basic validation of parsed items using cJSON type-check helpers
    if (!cJSON_IsNumber(a_item) || !cJSON_IsBool(b_item) || !cJSON_IsArray(arr_item)) {
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return 0;
    }

    // Get array size and first element
    int arr_size = cJSON_GetArraySize(arr_item);
    if (arr_size < 2) {
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return 0;
    }
    cJSON *first = cJSON_GetArrayItem(arr_item, 0);
    if (!cJSON_IsString(first)) {
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return 0;
    }

    // Demonstrate replacing an item in an array inside root (replace first number)
    cJSON *root_numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    if (cJSON_IsArray(root_numbers)) {
        // Replace index 0 with a new number
        cJSON_ReplaceItemInArray(root_numbers, 0, cJSON_CreateNumber(42));
    }

    // Cleanup parsed and duplicate structures (root owns the restored nested object)
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // If we've reached here, all API usages succeeded
    return 66;
}