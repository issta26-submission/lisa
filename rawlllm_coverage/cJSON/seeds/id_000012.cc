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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Build a JSON object:
    // {
    //   "name": "Alice",
    //   "age": 30,
    //   "spouse": null,
    //   "children": ["Bob", "Carol"],
    //   "address": { "city": "Metropolis", "zip": 12345 }
    // }
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Alice"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "spouse", cJSON_CreateNull());

    cJSON *children = cJSON_CreateArray();
    if (!children) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));
    cJSON_AddItemToArray(children, cJSON_CreateString("Carol"));
    cJSON_AddItemToObject(root, "children", children);

    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(address, "city", cJSON_CreateString("Metropolis"));
    cJSON_AddItemToObject(address, "zip", cJSON_CreateNumber(12345));
    cJSON_AddItemToObject(root, "address", address);

    // Render to string (compact)
    char *rendered = cJSON_PrintUnformatted(root);
    if (!rendered) {
        cJSON_Delete(root);
        return 0;
    }

    // Parse the rendered string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(rendered);
    free(rendered); // cJSON_Print* uses malloc, free is appropriate
    if (!parsed) {
        cJSON_Delete(root);
        return 0;
    }

    // Deep-duplicate the parsed object
    cJSON *dup = cJSON_Duplicate(parsed, 1); // 1 for deep copy
    if (!dup) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Add an extra field to the duplicate to show modification
    cJSON_AddItemToObject(dup, "name_full", cJSON_CreateString("Alice Smith"));

    // Detach and delete the second child from the duplicate's children array (if present)
    cJSON *dup_children = cJSON_GetObjectItemCaseSensitive(dup, "children");
    if (dup_children && cJSON_IsArray(dup_children)) {
        int sz = cJSON_GetArraySize(dup_children);
        if (sz > 1) {
            cJSON *detached = cJSON_DetachItemFromArray(dup_children, 1);
            if (detached) {
                cJSON_Delete(detached);
            }
        }
    }

    // Access some items to use the getters
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(dup, "name");
    cJSON *got_age = cJSON_GetObjectItemCaseSensitive(dup, "age");
    (void)got_name; // silence unused-variable warnings if any
    (void)got_age;

    // Print the modified duplicate (compact) and immediately free the buffer
    char *out = cJSON_PrintUnformatted(dup);
    if (out) free(out);

    // Cleanup all cJSON structures created
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence succeeded
    return 66;
}