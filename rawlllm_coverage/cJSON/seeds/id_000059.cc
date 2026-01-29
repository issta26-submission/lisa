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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Add simple items
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);

    // Create an array and add items to it
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));
    // Attach array to root (ownership transferred to root)
    cJSON_AddItemToObject(root, "list", arr);

    // Create a nested object and attach it
    cJSON *nested = cJSON_CreateObject();
    if (!nested) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToObject(nested, "a", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nested", nested);

    // Duplicate nested (deep copy) and add duplicate to root
    cJSON *dup_nested = cJSON_Duplicate(nested, /*recursive=*/1);
    if (!dup_nested) {
        cJSON_Delete(root);
        return -1;
    }
    cJSON_AddItemToObject(root, "dup", dup_nested);

    // Replace the "name" item with a new string
    cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("newname"));

    // Detach the "age" item from root (ownership transferred to us), then inspect and delete it
    cJSON *age_item = cJSON_DetachItemFromObject(root, "age");
    if (age_item) {
        if (cJSON_IsNumber(age_item)) {
            double age_val = age_item->valuedouble; (void)age_val; // example read
        }
        cJSON_Delete(age_item);
    }

    // Print unformatted JSON (compact) and pretty JSON
    char *compact = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);

    // Optionally inspect compact/pretty outputs (here we just parse the pretty output back)
    if (pretty) {
        cJSON *parsed = cJSON_Parse(pretty);
        // Free the pretty string after parsing
        cJSON_free(pretty);
        if (parsed) {
            // Inspect the parsed "list" array
            cJSON *parsed_list = cJSON_GetObjectItemCaseSensitive(parsed, "list");
            if (cJSON_IsArray(parsed_list)) {
                int sz = cJSON_GetArraySize(parsed_list);
                for (int i = 0; i < sz; ++i) {
                    cJSON *it = cJSON_GetArrayItem(parsed_list, i);
                    if (cJSON_IsNumber(it)) {
                        double v = it->valuedouble; (void)v;
                    } else if (cJSON_IsString(it)) {
                        const char *s = it->valuestring; (void)s;
                    }
                }
            }
            cJSON_Delete(parsed);
        }
    } else {
        // if pretty failed, still try to free compact if allocated below
        if (compact) {
            cJSON_free(compact);
            compact = NULL;
        }
    }

    // Free compact output if it was allocated
    if (compact) {
        cJSON_free(compact);
        compact = NULL;
    }

    // Demonstrate shallow duplicate of the whole root and delete it
    cJSON *shallow_dup = cJSON_Duplicate(root, /*recursive=*/0);
    if (shallow_dup) {
        cJSON_Delete(shallow_dup);
    }

    // Final cleanup of root and all its children
    cJSON_Delete(root);

    // If everything succeeded, return 66 as requested
    return 66;
}