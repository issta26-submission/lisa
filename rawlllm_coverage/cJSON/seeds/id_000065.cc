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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object and populate it
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 42);

    // Create an array and add items
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "items", arr);

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) { cJSON_Delete(root); return -1; }
    cJSON_AddStringToObject(nested, "k", "v");
    cJSON_AddItemToObject(root, "nested", nested);

    // Duplicate the created structure (demonstrates cJSON_Duplicate)
    cJSON *doc = cJSON_Duplicate(root, 1); // deep copy
    cJSON_Delete(root); // we can delete the original now
    if (!doc) return -1;

    // Access members and validate types/values
    cJSON *name = cJSON_GetObjectItemCaseSensitive(doc, "name");
    if (!cJSON_IsString(name) || !name->valuestring) { cJSON_Delete(doc); return -1; }

    cJSON *value = cJSON_GetObjectItemCaseSensitive(doc, "value");
    if (!cJSON_IsNumber(value) || value->valuedouble != 42) { cJSON_Delete(doc); return -1; }

    cJSON *items = cJSON_GetObjectItemCaseSensitive(doc, "items");
    if (!cJSON_IsArray(items)) { cJSON_Delete(doc); return -1; }

    int count = cJSON_GetArraySize(items);
    if (count != 2) { cJSON_Delete(doc); return -1; }

    cJSON *first = cJSON_GetArrayItem(items, 0);
    if (!cJSON_IsNumber(first) || first->valuedouble != 1) { cJSON_Delete(doc); return -1; }

    // Detach the first array item and reattach it at top-level as "detached_item"
    cJSON *detached = cJSON_DetachItemFromArray(items, 0);
    if (!detached) { cJSON_Delete(doc); return -1; }
    cJSON_AddItemToObject(doc, "detached_item", detached);

    // Detach the nested object and delete it to show detach + delete usage
    cJSON *nested_det = cJSON_DetachItemFromObject(doc, "nested");
    if (nested_det) {
        cJSON_Delete(nested_det);
    }

    // Replace (add) a new nested object
    cJSON *replacement = cJSON_CreateObject();
    if (!replacement) { cJSON_Delete(doc); return -1; }
    cJSON_AddStringToObject(replacement, "replaced_key", "replaced_value");
    cJSON_AddItemToObject(doc, "nested_replacement", replacement);

    // Parse a small JSON literal to demonstrate cJSON_Parse
    const char *literal = "{\"parsed_num\":123}";
    cJSON *parsed = cJSON_Parse(literal);
    if (!parsed) { cJSON_Delete(doc); return -1; }
    cJSON *pn = cJSON_GetObjectItemCaseSensitive(parsed, "parsed_num");
    if (!cJSON_IsNumber(pn) || pn->valuedouble != 123) { cJSON_Delete(parsed); cJSON_Delete(doc); return -1; }
    cJSON_Delete(parsed);

    // Detach and delete the previously reattached item to demonstrate cleanup
    cJSON *removed = cJSON_DetachItemFromObject(doc, "detached_item");
    if (removed) cJSON_Delete(removed);

    // Final cleanup
    cJSON_Delete(doc);

    return 66;
}