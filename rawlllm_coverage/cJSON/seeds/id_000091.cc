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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Exercise a broad sequence of cJSON APIs: creation, adding items,
    // arrays, duplication (deep/shallow), get, detach/replace/delete, and cleanup.
    cJSON *root = nullptr;
    cJSON *hobbies = nullptr;
    cJSON *copy = nullptr;
    cJSON *shallow = nullptr;

    root = cJSON_CreateObject();
    if (!root) return 0;

    // Basic items
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "alive", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "spouse", cJSON_CreateNull());

    // Array of strings
    hobbies = cJSON_CreateArray();
    if (!hobbies) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("swimming"));
    cJSON_AddItemToObject(root, "hobbies", hobbies);

    // Nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(address, "city", cJSON_CreateString("Berlin"));
    cJSON_AddItemToObject(address, "zip", cJSON_CreateNumber(12345));
    cJSON_AddItemToObject(root, "address", address);

    // Deep duplicate of the whole structure
    copy = cJSON_Duplicate(root, 1); // recurse = 1 (deep)
    if (!copy) { cJSON_Delete(root); return 0; }

    // Validate some items exist and have expected types in the copy
    cJSON *cop_name = cJSON_GetObjectItemCaseSensitive(copy, "name");
    if (!cop_name || !cJSON_IsString(cop_name)) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }

    cJSON *cop_hobbies = cJSON_GetObjectItemCaseSensitive(copy, "hobbies");
    if (!cop_hobbies || !cJSON_IsArray(cop_hobbies)) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }
    int hobby_count = cJSON_GetArraySize(cop_hobbies);
    if (hobby_count != 2) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }
    cJSON *first_hobby = cJSON_GetArrayItem(cop_hobbies, 0);
    if (!first_hobby || !cJSON_IsString(first_hobby)) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }

    // Detach an item from root and reattach under a different key
    cJSON *spouse = cJSON_DetachItemFromObject(root, "spouse");
    if (!spouse) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "former_spouse", spouse); // reattach moved item

    // Replace an item in the root (age -> 31)
    cJSON_ReplaceItemInObject(root, "age", cJSON_CreateNumber(31));

    // Delete an item from the hobbies array (delete second element)
    cJSON_DeleteItemFromArray(hobbies, 1);
    if (cJSON_GetArraySize(hobbies) != 1) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }

    // Create a shallow duplicate (no recursion)
    shallow = cJSON_Duplicate(root, 0); // recurse = 0 (shallow)
    if (!shallow) { cJSON_Delete(copy); cJSON_Delete(root); return 0; }

    // Cleanup all created structures (note: shallow and copy are independent; root owns its children)
    cJSON_Delete(shallow);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // If we reached this point, the sequence succeeded
    return 66;
}