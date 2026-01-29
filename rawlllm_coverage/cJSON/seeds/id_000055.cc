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
//<ID> 55
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

    // Add basic items: string, number, boolean
    cJSON_AddStringToObject(root, "name", "Example");
    cJSON_AddNumberToObject(root, "value", 42);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue()); // ownership transferred

    // Create an array and add several items
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));
    cJSON_AddItemToObject(root, "items", arr); // arr ownership transferred to root

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return -1; }
    cJSON_AddStringToObject(nested, "nested_key", "nested_value");
    cJSON_AddItemToObject(root, "nested", nested);

    // Serialize to a string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return -1; }

    // Parse the printed JSON to a new structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        cJSON_free(printed);
        cJSON_Delete(root);
        return -1;
    }

    // Inspect parsed items
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name_item) || strcmp(name_item->valuestring, "Example") != 0) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Deep-duplicate the parsed object
    cJSON *dup = cJSON_Duplicate(parsed, 1); // 1 => recurse / deep copy
    if (!dup) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Replace an item in the duplicate
    cJSON *replacement = cJSON_CreateString("Replaced");
    if (!replacement) {
        cJSON_free(printed);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }
    cJSON_ReplaceItemInObjectCaseSensitive(dup, "name", replacement); // ownership transferred

    // Detach first element from the original parsed array (if present) and reattach to duplicate
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (parsed_items && cJSON_IsArray(parsed_items) && cJSON_GetArraySize(parsed_items) > 0) {
        cJSON *detached = cJSON_DetachItemFromArray(parsed_items, 0); // ownership transferred to us
        if (detached) {
            cJSON_AddItemToObject(dup, "detached_item", detached); // ownership transferred to dup
        }
    }

    // Clean up: delete the original root, parsed and duplicate structures
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);

    // Free printed string
    cJSON_free(printed);

    // If we reached here, the sequence succeeded
    return 66;
}