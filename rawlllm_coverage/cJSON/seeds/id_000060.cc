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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic items using convenience functions
    cJSON_AddStringToObject(root, "name", "cJSON");
    cJSON_AddNumberToObject(root, "version", 1.7);
    cJSON_AddTrueToObject(root, "stable");

    // Create an array and add numeric items
    cJSON *numbers = cJSON_CreateArray();
    if (!numbers) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "numbers", numbers);

    // Create nested object
    cJSON *meta = cJSON_CreateObject();
    if (!meta) { cJSON_Delete(root); return -3; }
    cJSON_AddFalseToObject(meta, "experimental");
    cJSON_AddItemToObject(root, "meta", meta);

    // Print formatted JSON (allocates a string)
    char *printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return -4; }

    // Demonstrate cJSON_Minify by copying the printed string into a mutable buffer
    char *mutable_copy = (char*)malloc(strlen(printed) + 1);
    if (!mutable_copy) { free(printed); cJSON_Delete(root); return -5; }
    strcpy(mutable_copy, printed);
    cJSON_Minify(mutable_copy); // minifies in place
    free(mutable_copy);
    free(printed);

    // Deep-duplicate the whole structure
    cJSON *dup = cJSON_Duplicate(root, 1); // 1 = recurse / deep copy
    if (!dup) { cJSON_Delete(root); return -6; }

    // Access items in a case-sensitive way and check types
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -7;
    }

    // Replace the "name" item with a new string
    cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("libcjson"));

    // Detach the "numbers" array, operate on it, then add it back
    cJSON *detached_numbers = cJSON_DetachItemFromObject(root, "numbers");
    if (!detached_numbers) {
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -8;
    }
    // Replace the second element (index 1) with another number
    cJSON_ReplaceItemInArray(detached_numbers, 1, cJSON_CreateNumber(42));
    // Detach and free the first element (index 0)
    cJSON *removed = cJSON_DetachItemFromArray(detached_numbers, 0);
    if (removed) cJSON_Delete(removed);
    // Add the modified array back under the same key
    cJSON_AddItemToObject(root, "numbers", detached_numbers);

    // Add an item reference to the existing "meta" object (no duplicate)
    cJSON_AddItemReferenceToObject(root, "meta_ref", meta);

    // Parse a JSON string and inspect its contents
    const char *sample = "{\"a\":123, \"b\": [true, false, null]}";
    cJSON *parsed = cJSON_Parse(sample);
    if (parsed) {
        cJSON *a = cJSON_GetObjectItemCaseSensitive(parsed, "a");
        if (cJSON_IsNumber(a)) {
            // value available as a->valuedouble or a->valueint
            (void)a->valuedouble; // avoid unused-variable warnings
        }
        cJSON_Delete(parsed);
    }

    // Clean-up duplicates and root (which also frees children)
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // If we reached here, sequence succeeded
    return 66;
}