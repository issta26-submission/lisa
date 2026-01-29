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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API usages:
    // - create objects/arrays/primitives
    // - serialize to string
    // - parse back
    // - inspect, replace, detach, duplicate
    // - cleanup
    //
    // Returns 66 on overall success, non-66 on failure.

    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 1;
    }

    // Add simple members
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddItemToObject(root, "is_active", cJSON_CreateTrue()); // boolean
    cJSON_AddItemToObject(root, "notes", cJSON_CreateNull());     // explicit null

    // Create and populate an array
    cJSON *scores = cJSON_CreateArray();
    if (!scores) {
        cJSON_Delete(root);
        return 2;
    }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(87.5));
    cJSON_AddItemToArray(scores, cJSON_CreateString("N/A"));
    cJSON_AddItemToObject(root, "scores", scores);

    // Create a nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) {
        cJSON_Delete(root);
        return 3;
    }
    cJSON_AddStringToObject(address, "city", "Testville");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Serialize to string
    char *printed = cJSON_Print(root);
    if (!printed) {
        cJSON_Delete(root);
        return 4;
    }

    // Parse the serialized JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        // cleanup
        free(printed);
        cJSON_Delete(root);
        return 5;
    }

    // Inspect some members from the parsed object
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 6;
    }

    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 7;
    }

    // Replace the age with a new value (demonstrates Replace API)
    cJSON_ReplaceItemInObject(parsed, "age", cJSON_CreateNumber(31));

    // Add a new boolean field using AddFalse convenience API
    cJSON_AddFalseToObject(parsed, "verified");

    // Detach the "notes" item from parsed object (returns the detached item)
    cJSON *detached_notes = cJSON_DetachItemFromObject(parsed, "notes");
    if (detached_notes) {
        // we no longer want it; delete it explicitly
        cJSON_Delete(detached_notes);
    }

    // Duplicate the parsed structure (deep copy) and then free the duplicate
    cJSON *dup = cJSON_Duplicate(parsed, /*recurse=*/1);
    if (!dup) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 8;
    }
    cJSON_Delete(dup);

    // Clean up all remaining allocations
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(root);

    // Success: return the requested value
    return 66;
}