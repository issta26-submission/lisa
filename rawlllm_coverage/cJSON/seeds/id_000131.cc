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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of cJSON API calls that:
    // - create objects/arrays/values
    // - print (pretty and compact)
    // - parse back
    // - duplicate and compare
    // - replace and detach/reattach items
    // - minify and parse the minified text
    // - clean up all allocated resources
    //
    // Returns 66 on success. On any failure returns a non-66 error code.

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 1;
    }

    // Basic properties
    cJSON_AddStringToObject(root, "name", "Example");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddItemToObject(root, "married", cJSON_CreateTrue());

    // Array of children
    cJSON *children = cJSON_CreateArray();
    if (!children) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToArray(children, cJSON_CreateString("Alice"));
    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));
    cJSON_AddItemToObject(root, "children", children);

    // Nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 3; }
    cJSON_AddStringToObject(address, "street", "123 Main St");
    cJSON_AddStringToObject(address, "city", "Anytown");
    cJSON_AddItemToObject(root, "address", address);

    // Print pretty and compact JSON
    char *printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return 4; }
    char *printed_unformatted = cJSON_PrintUnformatted(root);
    if (!printed_unformatted) { free(printed); cJSON_Delete(root); return 5; }

    // Parse the compact JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed_unformatted);
    if (!parsed) {
        free(printed);
        free(printed_unformatted);
        cJSON_Delete(root);
        return 6;
    }

    // Duplicate and compare
    cJSON *dup = cJSON_Duplicate(parsed, 1); // deep copy
    if (!dup) {
        cJSON_Delete(parsed);
        free(printed);
        free(printed_unformatted);
        cJSON_Delete(root);
        return 7;
    }
    if (!cJSON_Compare(parsed, dup, 1)) { // case-sensitive compare
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        free(printed);
        free(printed_unformatted);
        cJSON_Delete(root);
        return 8;
    }

    // Replace "age" in parsed with a new number (old item freed by API)
    cJSON_ReplaceItemInObject(parsed, "age", cJSON_CreateNumber(31));

    // Detach "children" from parsed and attach it to the original root under a new key
    cJSON *detachedChildren = cJSON_DetachItemFromObject(parsed, "children");
    if (detachedChildren) {
        cJSON_AddItemToObject(root, "detached_children_from_parsed", detachedChildren);
        // ownership transferred to root; do not delete detachedChildren separately
    }

    // Minify the pretty-printed JSON in-place and parse it
    cJSON_Minify(printed); // modifies printed buffer in place
    cJSON *parsedMin = cJSON_Parse(printed);
    if (!parsedMin) {
        // cleanup on failure
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        free(printed);
        free(printed_unformatted);
        cJSON_Delete(root);
        return 9;
    }

    // Cleanup all allocated memory and objects.
    free(printed);
    free(printed_unformatted);
    cJSON_Delete(parsedMin);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root); // also deletes detachedChildren since it was attached to root

    return 66;
}