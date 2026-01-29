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
//<ID> 64
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

    // Add simple properties
    if (!cJSON_AddStringToObject(root, "name", "example")) {
        cJSON_Delete(root);
        return -2;
    }
    if (!cJSON_AddNumberToObject(root, "value", 123.45)) {
        cJSON_Delete(root);
        return -3;
    }

    // Create an array and populate it
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return -4;
    }
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());

    // Add the array to the root object
    cJSON_AddItemToObject(root, "items", arr);

    // Deep-duplicate the whole structure
    cJSON *dup = cJSON_Duplicate(root, 1); // 1 => recursive (deep) duplicate
    if (!dup) {
        cJSON_Delete(root);
        return -5;
    }

    // Print both versions (pretty and unformatted)
    char *printed_pretty = cJSON_Print(root);
    char *printed_unf = cJSON_PrintUnformatted(dup);
    if (!printed_pretty || !printed_unf) {
        if (printed_pretty) cJSON_free(printed_pretty);
        if (printed_unf) cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -6;
    }

    // Parse the unformatted JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed_unf);
    if (!parsed) {
        cJSON_free(printed_pretty);
        cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -7;
    }

    // Inspect parsed content with type checks
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON *value = cJSON_GetObjectItemCaseSensitive(parsed, "value");
    cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!cJSON_IsString(name) || !cJSON_IsNumber(value) || !cJSON_IsArray(items)) {
        cJSON_Delete(parsed);
        cJSON_free(printed_pretty);
        cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -8;
    }

    // Check array size and one element's type/value
    int arr_size = cJSON_GetArraySize(items);
    if (arr_size != 3) {
        cJSON_Delete(parsed);
        cJSON_free(printed_pretty);
        cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -9;
    }
    cJSON *second = cJSON_GetArrayItem(items, 1);
    if (!cJSON_IsNumber(second) || second->valuedouble != 2) {
        cJSON_Delete(parsed);
        cJSON_free(printed_pretty);
        cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -10;
    }

    // Replace the "value" in the duplicated object with a new number
    cJSON *new_value = cJSON_CreateNumber(999);
    if (!new_value) {
        cJSON_Delete(parsed);
        cJSON_free(printed_pretty);
        cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -11;
    }
    // This will delete the old "value" item inside dup and set the new one
    if (!cJSON_ReplaceItemInObject(dup, "value", new_value)) {
        // On failure, delete the created new_value (Replace would have taken ownership on success)
        cJSON_Delete(new_value);
        cJSON_Delete(parsed);
        cJSON_free(printed_pretty);
        cJSON_free(printed_unf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -12;
    }

    // Detach the "name" item from dup and then delete it explicitly
    cJSON *detached_name = cJSON_DetachItemFromObject(dup, "name");
    if (detached_name) {
        cJSON_Delete(detached_name);
    } else {
        // If detach failed, continue but signal a non-fatal error (cleanup will still occur)
        // not returning here because it's not critical for the demonstration
    }

    // Demonstrate cJSON_Duplicate on a child (shallow duplicate)
    cJSON *items_dup_shallow = cJSON_Duplicate(items, 0); // shallow duplicate of parsed->items
    if (items_dup_shallow) {
        cJSON_Delete(items_dup_shallow);
    }

    // Cleanup everything
    cJSON_Delete(parsed);
    cJSON_free(printed_pretty);
    cJSON_free(printed_unf);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // If we reached here, the sequence of APIs executed successfully
    return 66;
}