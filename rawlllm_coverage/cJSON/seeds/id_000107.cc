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
//<ID> 107
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

    // Add various item types using convenience helpers
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 123.456);
    cJSON_AddTrueToObject(root, "alive");
    cJSON_AddNullToObject(root, "nothing");

    // Create an array and add it to root
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(nested, "inner", "yes");
    cJSON_AddItemToObject(root, "nested", nested);

    // Deep duplicate the whole tree
    cJSON *copy = cJSON_Duplicate(root, 1); // recurse = 1 (true)
    if (!copy) { cJSON_Delete(root); return 0; }

    // Print the duplicate to an unformatted JSON string
    char *printed = cJSON_PrintUnformatted(copy);
    if (!printed) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }

    // Parse the printed string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) { free(printed); cJSON_Delete(copy); cJSON_Delete(root); return 0; }

    // Access items from parsed tree and verify a value
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!name || !cJSON_IsString(name) || !name->valuestring || std::strcmp(name->valuestring, "example") != 0) {
        // cleanup on failure
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }

    // Detach the array from parsed and delete it explicitly
    cJSON *detached_array = cJSON_DetachItemFromObject(parsed, "array");
    if (detached_array) cJSON_Delete(detached_array);

    // Replace the numeric "value" with a new number
    cJSON *newnum = cJSON_CreateNumber(999.0);
    if (!newnum) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }
    cJSON_ReplaceItemInObject(parsed, "value", newnum);

    // Remove the nested object from parsed
    cJSON_DeleteItemFromObject(parsed, "nested");

    // All operations completed successfully -> cleanup and return success code 66
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    return 66;
}