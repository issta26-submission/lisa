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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add basic types
    cJSON_AddStringToObject(root, "name", "test");
    cJSON_AddNumberToObject(root, "value", 123);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull());

    // Create and add a nested object
    cJSON *child = cJSON_CreateObject();
    if (!child) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(child, "child_name", "nested");
    cJSON_AddItemToObject(root, "child", child);

    // Create and add an array
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToObject(root, "arr", arr);

    // Print the JSON to a string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return 0; }

    // Parse the printed string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    cJSON_free(printed); // free the printed string
    if (!parsed) { cJSON_Delete(root); return 0; }

    // Validate some items exist and have expected types
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!name || !cJSON_IsString(name)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    cJSON *value = cJSON_GetObjectItemCaseSensitive(parsed, "value");
    if (!value || !cJSON_IsNumber(value)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    // Detach a child from the parsed object and reattach it to the original root
    cJSON *detached_child = cJSON_DetachItemFromObject(parsed, "child");
    if (detached_child) {
        cJSON_AddItemToObject(root, "moved_child", detached_child);
    }

    // Duplicate the parsed object (deep copy), modify it, and attach to root
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1); // deep duplicate
    if (!dup_parsed) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    cJSON_ReplaceItemInObject(dup_parsed, "value", cJSON_CreateNumber(999));
    cJSON_AddItemToObject(root, "parsed_copy", dup_parsed);

    // Demonstrate a shallow duplicate, then free it (do NOT attach it back to the same tree)
    cJSON *shallow = cJSON_Duplicate(root, 0); // shallow duplicate
    if (!shallow) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    cJSON_Delete(shallow);

    // Print final root and free the printed buffer
    char *final_print = cJSON_PrintUnformatted(root);
    if (!final_print) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    cJSON_free(final_print);

    // Clean up: deleting parsed and root. Note: root owns items added to it (including dup_parsed and moved_child).
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reach here, the sequence succeeded
    return 66;
}