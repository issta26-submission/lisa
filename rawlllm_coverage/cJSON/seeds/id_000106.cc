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
//<ID> 106
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
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    // Create and populate an array, then add it to the root object
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "items", arr);

    // Print to an unformatted string and parse it back
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return 0; }
    cJSON *parsed = cJSON_Parse(printed);
    free(printed); // cJSON_Print* uses malloc; free the returned string
    if (!parsed) { cJSON_Delete(root); return 0; }

    // Verify an item in parsed structure
    cJSON *countItem = cJSON_GetObjectItemCaseSensitive(parsed, "count");
    if (!cJSON_IsNumber(countItem) || countItem->valueint != 3) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return 0;
    }

    // Deep-duplicate parsed
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return 0;
    }

    // Replace an item in parsed, detach another and attach it into the duplicate
    cJSON_ReplaceItemInObject(parsed, "name", cJSON_CreateString("newname"));
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "active");
    if (detached) {
        cJSON_AddItemToObject(dup, "moved_active", detached);
    }

    // Delete an item from the duplicate (demonstrate deletion API)
    cJSON_DeleteItemFromObject(dup, "count");

    // Create a shallow duplicate of root
    cJSON *shallow = cJSON_Duplicate(root, 0);
    if (!shallow) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        cJSON_Delete(dup);
        return 0;
    }

    // Clean up all allocated cJSON structures
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(shallow);

    // Signal success as requested
    return 66;
}