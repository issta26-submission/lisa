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
//<ID> 0
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
    if (!cJSON_AddItemToObject(root, "pi", cJSON_CreateNumber(3.1415926))) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(root, "name", cJSON_CreateString("cJSON usage test"))) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue())) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull())) { cJSON_Delete(root); return 0; }

    // Create an array and populate it with numbers and an embedded object
    cJSON *array = cJSON_CreateArray();
    if (!array) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(root, "items", array)) { cJSON_Delete(array); cJSON_Delete(root); return 0; }

    if (!cJSON_AddItemToArray(array, cJSON_CreateNumber(1))) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToArray(array, cJSON_CreateNumber(2))) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToArray(array, cJSON_CreateNumber(3))) { cJSON_Delete(root); return 0; }

    // Create a nested object and add it into the array
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(nested, "childName", cJSON_CreateString("childValue"))) { cJSON_Delete(nested); cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToArray(array, nested)) { cJSON_Delete(nested); cJSON_Delete(root); return 0; }

    // Serialize (unformatted) to a string and free it
    char *out = cJSON_PrintUnformatted(root);
    if (!out) { cJSON_Delete(root); return 0; }
    free(out);
    out = NULL;

    // Parse a JSON string
    const char *json_text = "{\"parsed\": true, \"list\": [10, 20, 30]}";
    cJSON *parsed = cJSON_Parse(json_text);
    if (!parsed) { cJSON_Delete(root); return 0; }

    // Access parsed contents
    cJSON *plist = cJSON_GetObjectItemCaseSensitive(parsed, "list");
    if (!plist || !cJSON_IsArray(plist)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    int list_size = cJSON_GetArraySize(plist);
    if (list_size != 3) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    for (int i = 0; i < list_size; ++i) {
        cJSON *elem = cJSON_GetArrayItem(plist, i);
        if (!elem || !cJSON_IsNumber(elem)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    }

    // Duplicate the root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    // Replace an item in the duplicate
    if (!cJSON_ReplaceItemInObject(dup, "name", cJSON_CreateString("duplicated"))) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Detach an item from the original and move it into the duplicate
    cJSON *detached = cJSON_DetachItemFromObject(root, "nothing"); // ownership transferred
    if (detached) {
        if (!cJSON_AddItemToObject(dup, "detached", detached)) {
            // If adding failed, delete detached to avoid leak
            cJSON_Delete(detached);
            cJSON_Delete(dup);
            cJSON_Delete(parsed);
            cJSON_Delete(root);
            return 0;
        }
    }

    // Verify an item in the duplicate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(dup, "name");
    if (!name_item || !cJSON_IsString(name_item) || (name_item->valuestring == NULL)) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Clean up
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Indicate success as requested
    return 66;
}