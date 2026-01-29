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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls.
    // It builds a JSON object, serializes it, parses it back, inspects and
    // manipulates items, then cleans up. Returns 66 on success.
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add various item types
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddItemToObject(root, "value", cJSON_CreateNumber(42));
    cJSON_AddItemToObject(root, "valid", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull());

    // Create an array and add elements
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(arr, cJSON_CreateFalse());
    cJSON_AddItemToObject(root, "items", arr);

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return 3; }
    cJSON_AddItemToObject(nested, "nested_key", cJSON_CreateString("nested_value"));
    cJSON_AddItemToObject(root, "nested", nested);

    // Serialize to string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return 4; }

    // Parse the serialized string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    cJSON_free(printed); // free the printed buffer allocated by cJSON_Print*
    if (!parsed) { cJSON_Delete(root); return 5; }

    // Inspect items from the parsed object
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 6;
    }
    cJSON *value = cJSON_GetObjectItem(parsed, "value");
    if (!cJSON_IsNumber(value)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 7;
    }

    // Detach the array from parsed object (so we can manage it separately)
    cJSON *items = cJSON_DetachItemFromObject(parsed, "items");
    if (!items || !cJSON_IsArray(items)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 8;
    }

    // Duplicate the parsed object (deep copy) and modify the duplicate
    cJSON *dup = cJSON_Duplicate(parsed, 1); // 1 => recursive duplicate
    if (!dup) {
        cJSON_Delete(items);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 9;
    }
    // Replace an item in the duplicate
    cJSON_ReplaceItemInObject(dup, "value", cJSON_CreateNumber(100));

    // Optionally serialize the duplicate (and free the buffer)
    char *dup_printed = cJSON_Print(dup);
    if (dup_printed) cJSON_free(dup_printed);

    // Cleanup: delete detached items and all cJSON structures we created
    cJSON_Delete(items);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    return 66;
}