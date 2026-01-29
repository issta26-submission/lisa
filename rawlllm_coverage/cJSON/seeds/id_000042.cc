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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add several types: string, number, true, null
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 42);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddNullToObject(root, "maybe");

    // Create a nested object and attach it
    cJSON *child = cJSON_CreateObject();
    if (!child) { cJSON_Delete(root); return -1; }
    cJSON_AddStringToObject(child, "childName", "inner");
    cJSON_AddItemToObject(root, "child", child); // child now owned by root

    // Create an array of numbers and attach it
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "numbers", arr);

    // Serialize (unformatted) the structure to a string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return -1; }

    // Parse the serialized string back to a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    free(printed); // free the printed string
    if (!parsed) { cJSON_Delete(root); return -1; }

    // Duplicate the parsed object (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) { cJSON_Delete(parsed); cJSON_Delete(root); return -1; }

    // Replace the "value" number in the duplicate
    cJSON_ReplaceItemInObject(dup, "value", cJSON_CreateNumber(100));

    // Detach the second element (index 1) from the "numbers" array in the duplicate and delete it
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(dup, "numbers");
    if (cJSON_IsArray(numbers)) {
        cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
        if (detached) cJSON_Delete(detached);
    }

    // Pretty-print the modified duplicate, then minify the printed JSON in-place
    char *printed2 = cJSON_Print(dup);
    if (!printed2) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return -1; }
    cJSON_Minify(printed2);

    // Parse the minified string to verify correctness
    cJSON *parsed2 = cJSON_Parse(printed2);
    free(printed2);
    if (!parsed2) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return -1; }

    // Verify presence of some expected items
    if (!cJSON_HasObjectItem(parsed2, "name") || !cJSON_HasObjectItem(parsed2, "numbers")) {
        cJSON_Delete(parsed2);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Clean up all allocated cJSON structures
    cJSON_Delete(parsed2);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Indicate success as requested
    return 66;
}