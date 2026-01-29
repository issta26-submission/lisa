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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Helper duplicate of strdup to avoid non-portable strdup usage
    auto my_strdup = [](const char *s) -> char* {
        if (!s) return nullptr;
        size_t n = std::strlen(s) + 1;
        char *p = (char*)std::malloc(n);
        if (p) std::memcpy(p, s, n);
        return p;
    };

    // Root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add simple items
    if (!cJSON_AddStringToObject(root, "name", "cJSON")) { cJSON_Delete(root); return 2; }
    if (!cJSON_AddNumberToObject(root, "age", 10))    { cJSON_Delete(root); return 3; }
    if (!cJSON_AddBoolToObject(root, "active", 1))   { cJSON_Delete(root); return 4; }
    if (!cJSON_AddNullToObject(root, "nothing"))     { cJSON_Delete(root); return 5; }

    // Create an array and add to root
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 6; }
    if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(1))) { cJSON_Delete(root); return 7; }
    if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(2))) { cJSON_Delete(root); return 8; }
    if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(3))) { cJSON_Delete(root); return 9; }
    // Add array to root (takes ownership of arr)
    cJSON_AddItemToObject(root, "numbers", arr);

    // Nested object
    cJSON *child = cJSON_CreateObject();
    if (!child) { cJSON_Delete(root); return 10; }
    cJSON_AddStringToObject(child, "child_name", "inner");
    cJSON_AddItemToObject(root, "child", child);

    // Print unformatted JSON
    char *unfmt = cJSON_PrintUnformatted(root);
    if (!unfmt) { cJSON_Delete(root); return 11; }

    // Parse the printed JSON back
    cJSON *parsed = cJSON_Parse(unfmt);
    std::free(unfmt);
    if (!parsed) { cJSON_Delete(root); return 12; }

    // Duplicate the parsed structure (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1); // recursive = 1
    if (!dup) { cJSON_Delete(parsed); cJSON_Delete(root); return 13; }

    // Replace an item in the duplicate
    if (!cJSON_ReplaceItemInObject(dup, "age", cJSON_CreateNumber(20))) {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 14;
    }

    // Detach an item from the duplicate and re-add it to the original root
    cJSON *detached = cJSON_DetachItemFromObject(dup, "nothing");
    if (detached) {
        // Transfer ownership of detached node to root under a new key
        cJSON_AddItemToObject(root, "detached_from_dup", detached);
        // Note: do NOT delete detached here; root now owns it
    }

    // Work with the array: replace first element, insert another
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    if (!numbers) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 15; }

    if (!cJSON_ReplaceItemInArray(numbers, 0, cJSON_CreateNumber(100))) {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 16;
    }

    if (!cJSON_InsertItemInArray(numbers, 1, cJSON_CreateNumber(200))) {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 17;
    }

    // Query array size and items
    int arr_size = cJSON_GetArraySize(numbers);
    if (arr_size < 3) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 18; }

    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    if (!first_item || first_item->valuedouble != 100) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 19; }

    cJSON *second_item = cJSON_GetArrayItem(numbers, 1);
    if (!second_item || second_item->valuedouble != 200) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 20; }

    // Test cJSON_Minify on a raw JSON string
    char *to_minify = my_strdup("{ \"x\": 1, \"y\": 2 }");
    if (!to_minify) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 21; }
    cJSON_Minify(to_minify);
    cJSON *min_parsed = cJSON_Parse(to_minify);
    std::free(to_minify);
    if (!min_parsed) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 22; }
    cJSON_Delete(min_parsed);

    // Access a string field in parsed copy
    cJSON *p_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!p_name || !cJSON_IsString(p_name) || !p_name->valuestring) {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 23;
    }

    // Clean up: parsed and dup are independent trees; root owns its own nodes.
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // Everything succeeded
    return 66;
}