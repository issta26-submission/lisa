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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add simple members
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 42);
    cJSON_AddItemToObject(root, "married", cJSON_CreateTrue());

    // Add an array of children
    cJSON *children = cJSON_CreateArray();
    if (!children) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToArray(children, cJSON_CreateString("Alice"));
    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));
    cJSON_AddItemToObject(root, "children", children);

    // Add nested object (address)
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 3; }
    cJSON_AddStringToObject(address, "city", "New York");
    cJSON_AddNumberToObject(address, "zip", 10001);
    cJSON_AddItemToObject(root, "address", address);

    // Serialize (pretty)
    char *printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return 4; }
    // Basic sanity check on printed output
    if (strlen(printed) == 0) { free(printed); cJSON_Delete(root); return 5; }

    // Parse back from the serialized string
    cJSON *parsed = cJSON_Parse(printed);
    free(printed);
    if (!parsed) { cJSON_Delete(root); return 6; }

    // Validate parsed members
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name_item) || (name_item->valuestring == NULL)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 7;
    }
    cJSON *age_item = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age_item)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 8;
    }

    // Duplicate the parsed object (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 9;
    }

    // Replace age in duplicate
    cJSON_ReplaceItemInObject(dup, "age", cJSON_CreateNumber(43));

    // Work with the children array in the duplicate
    cJSON *dup_children = cJSON_GetObjectItemCaseSensitive(dup, "children");
    if (!cJSON_IsArray(dup_children)) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 10;
    }

    // Remove the first child ("Alice")
    cJSON_DeleteItemFromArray(dup_children, 0);

    // Insert a new child at index 0
    cJSON_InsertItemInArray(dup_children, 0, cJSON_CreateString("Charlie"));

    // Replace the address object with a simple string
    cJSON_ReplaceItemInObject(dup, "address", cJSON_CreateString("hidden"));

    // Check the array size and a member of it
    int size = cJSON_GetArraySize(dup_children);
    if (size <= 0) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 11;
    }
    cJSON *first_child = cJSON_GetArrayItem(dup_children, 0);
    if (!cJSON_IsString(first_child)) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 12;
    }

    // Produce an unformatted JSON string from the modified duplicate
    char *flat = cJSON_PrintUnformatted(dup);
    if (!flat) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 13;
    }
    free(flat);

    // Add a boolean false to duplicate
    cJSON_AddItemToObject(dup, "verified", cJSON_CreateFalse());

    // Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence succeeded
    return 66;
}