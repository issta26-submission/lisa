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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add basic types
    if (!cJSON_AddStringToObject(root, "name", "ChatGPT")) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddNumberToObject(root, "version", 4.0)) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());   // transfers ownership
    cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull());   // transfers ownership

    // Create and add a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddStringToObject(nested, "inner", "value")) { cJSON_Delete(nested); cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "nested", nested); // nested now owned by root

    // Create an array and add different item types
    cJSON *array = cJSON_CreateArray();
    if (!array) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(array, cJSON_CreateString("one"));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(array, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "list", array); // array now owned by root

    // Print unformatted (compact) JSON to a string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return 0; }

    // Parse back the printed JSON
    cJSON *parsed = cJSON_Parse(printed);
    free(printed);
    if (!parsed) { cJSON_Delete(root); return 0; }

    // Duplicate the parsed object (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    // Validate some items on the duplicated object
    cJSON *name = cJSON_GetObjectItemCaseSensitive(dup, "name");
    if (!cJSON_IsString(name) || name->valuestring == NULL) {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 0;
    }

    cJSON *list = cJSON_GetObjectItemCaseSensitive(dup, "list");
    if (!cJSON_IsArray(list)) {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 0;
    }

    // Inspect array contents via index access
    int arr_size = cJSON_GetArraySize(list);
    for (int i = 0; i < arr_size; ++i) {
        cJSON *elem = cJSON_GetArrayItem(list, i);
        if (!elem) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
        // No further checks required here; demonstration of access is enough
    }

    // Delete an item from the duplicated object (case-sensitive)
    cJSON_DeleteItemFromObjectCaseSensitive(dup, "nothing");

    // Replace the version number with a new value
    cJSON_ReplaceItemInObjectCaseSensitive(dup, "version", cJSON_CreateNumber(5.0));

    // Detach the list from duplicated object (ownership transferred to detached pointer)
    cJSON *detached_list = cJSON_DetachItemFromObjectCaseSensitive(dup, "list");
    if (detached_list) {
        // We can inspect or delete it; delete to avoid leak
        cJSON_Delete(detached_list);
    }

    // Print the modified duplicate (pretty) and free the string
    char *pretty = cJSON_Print(dup);
    if (pretty) free(pretty);

    // Clean up all allocated cJSON structures
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence succeeded
    return 66;
}