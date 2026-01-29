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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Note: this function assumes that the cJSON header is included
    // and that cJSON_malloc/cJSON_free are available as provided by the library.
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Basic additions
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddTrueToObject(root, "student");
    cJSON_AddNullToObject(root, "spouse");

    // Create array and add items
    cJSON *hobbies = cJSON_CreateArray();
    if (!hobbies) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToObject(root, "hobbies", hobbies);
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("coding"));

    // Nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return -3; }
    cJSON_AddStringToObject(address, "city", "Exampleville");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Duplicate the whole tree (deep copy)
    cJSON *copy = cJSON_Duplicate(root, 1);
    if (!copy) { cJSON_Delete(root); return -4; }

    // Print (unformatted) and free the resulting string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(copy); cJSON_Delete(root); return -5; }
    cJSON_free(printed);

    // Parse a JSON text and inspect it
    const char *json_text = "{\"parsed\":true,\"numbers\":[1,2,3]}";
    cJSON *parsed = cJSON_Parse(json_text);
    if (!parsed) { cJSON_Delete(copy); cJSON_Delete(root); return -6; }
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(parsed, "numbers");
    if (!cJSON_IsArray(numbers) || cJSON_GetArraySize(numbers) != 3) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return -7;
    }
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    if (!cJSON_IsNumber(first_num)) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return -8;
    }

    // Replace the "age" value with a new number
    cJSON_ReplaceItemInObject(root, "age", cJSON_CreateNumber(31));

    // Detach the "spouse" item from root and add it to hobbies (if null, add an explicit null)
    cJSON *detached_spouse = cJSON_DetachItemFromObject(root, "spouse");
    if (detached_spouse) {
        cJSON_AddItemToArray(hobbies, detached_spouse);
    } else {
        cJSON_AddItemToArray(hobbies, cJSON_CreateNull());
    }

    // Replace first hobby with a new string
    if (cJSON_GetArraySize(hobbies) > 0) {
        cJSON_ReplaceItemInArray(hobbies, 0, cJSON_CreateString("writing"));
    }

    // Add a reference to the address object into hobbies (demonstrates AddItemReference*)
    cJSON *addr_ref = cJSON_GetObjectItemCaseSensitive(root, "address");
    if (addr_ref) {
        cJSON_AddItemReferenceToArray(hobbies, addr_ref);
    }

    // Remove the second hobby (if exists) and delete it explicitly
    if (cJSON_GetArraySize(hobbies) > 1) {
        cJSON *removed = cJSON_DetachItemFromArray(hobbies, 1);
        cJSON_Delete(removed);
    }

    // Validate that "name" is present and is a string
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(name) || !name->valuestring) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return -9;
    }

    // Cleanup parsed and duplicated trees; deleting root will clean up all items attached to it
    cJSON_Delete(parsed);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // Success as requested
    return 66;
}