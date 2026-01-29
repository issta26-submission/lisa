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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls:
    // - create objects/arrays/values
    // - add items to objects/arrays
    // - print and parse JSON text
    // - get/inspect items
    // - detach/delete items
    // - duplicate a tree
    // - clean up all allocated memory
    //
    // On success it returns 66 as requested. On failure it returns a non-66 value.

    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic types
    cJSON_AddStringToObject(root, "name", "John Doe");
    cJSON_AddNumberToObject(root, "age", 30);
    // Add a boolean value by creating a true/false node and attaching it
    cJSON_AddItemToObject(root, "married", cJSON_CreateTrue());

    // Create and add an array
    cJSON *phones = cJSON_CreateArray();
    if (!phones) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(phones, cJSON_CreateString("123-4567"));
    cJSON_AddItemToArray(phones, cJSON_CreateString("987-6543"));
    cJSON_AddItemToObject(root, "phones", phones); // phones now owned by root

    // Create a nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return -3; }
    cJSON_AddStringToObject(address, "city", "Exampleville");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address); // address now owned by root

    // Convert to compact JSON string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return -4; }

    // (Optional) show the JSON string -- in a real test you might assert contents
    // printf("printed JSON: %s\n", printed);

    // Parse the JSON text back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    // free the printed text (cJSON_PrintUnformatted uses malloc)
    free(printed);
    if (!parsed) { cJSON_Delete(root); return -5; }

    // Inspect some items in parsed tree
    cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(pname) || (pname->valuestring == NULL)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -6;
    }

    cJSON *page = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(page)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -7;
    }

    cJSON *pphones = cJSON_GetObjectItemCaseSensitive(parsed, "phones");
    if (!cJSON_IsArray(pphones)) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -8;
    }

    // Detach the age item from parsed and delete it separately (example of detach)
    cJSON *detached_age = cJSON_DetachItemFromObject(parsed, "age");
    if (detached_age) {
        cJSON_Delete(detached_age);
        // Add an updated age value
        cJSON_AddNumberToObject(parsed, "age", 31);
    } else {
        // If age wasn't found, still add one
        cJSON_AddNumberToObject(parsed, "age", 31);
    }

    // Duplicate the parsed JSON (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1); // 1 => recurse/deep copy
    if (!dup) {
        cJSON_Delete(root);
        cJSON_Delete(parsed);
        return -9;
    }

    // Work with the duplicated array: remove the first phone (demonstrate array deletion)
    cJSON *dup_phones = cJSON_GetObjectItemCaseSensitive(dup, "phones");
    if (cJSON_IsArray(dup_phones) && cJSON_GetArraySize(dup_phones) > 0) {
        cJSON_DeleteItemFromArray(dup_phones, 0);
    }

    // Optionally convert duplicate to string (not required)
    char *dup_text = cJSON_PrintUnformatted(dup);
    if (dup_text) {
        // printf("duplicate JSON after modification: %s\n", dup_text);
        free(dup_text);
    }

    // Cleanup all created cJSON structures
    cJSON_Delete(root);   // deletes root and everything that was attached to it
    cJSON_Delete(parsed); // parsed is a separate tree; delete it
    cJSON_Delete(dup);    // delete duplicate tree

    // If we reached here, assume success
    return 66;
}