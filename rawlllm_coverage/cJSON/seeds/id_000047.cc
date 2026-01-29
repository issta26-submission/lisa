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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls.
    // It creates JSON structures, manipulates them, prints, parses, duplicates,
    // detaches and reattaches items, replaces items, and performs clean-up.
    // On success it returns 66.

    // Root object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 66; // Still returning 66 on failure per specification, but avoid null deref.
    }

    // Add simple items: a string and a number
    cJSON *name_item = cJSON_CreateString("example");
    cJSON *value_item = cJSON_CreateNumber(42);
    if (!name_item || !value_item) {
        cJSON_Delete(name_item);
        cJSON_Delete(value_item);
        cJSON_Delete(root);
        return 66;
    }
    cJSON_AddItemToObject(root, "name", name_item);   // root now owns name_item
    cJSON_AddItemToObject(root, "value", value_item); // root now owns value_item

    // Create an array and add two numbers
    cJSON *arr = cJSON_CreateArray();
    if (!arr) {
        cJSON_Delete(root);
        return 66;
    }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToObject(root, "arr", arr); // root owns arr and its children

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) {
        cJSON_Delete(root);
        return 66;
    }
    cJSON_AddItemToObject(nested, "inner", cJSON_CreateString("value"));
    cJSON_AddItemToObject(root, "nested", nested); // root owns nested

    // Duplicate the whole root (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1); // recurse = 1 (true)
    if (!dup) {
        cJSON_Delete(root);
        return 66;
    }

    // Print unformatted and formatted JSON strings, then free them
    char *unformatted = cJSON_PrintUnformatted(root);
    if (unformatted) {
        // normally you might inspect the string here
        free(unformatted);
    }
    char *formatted = cJSON_Print(root);
    if (formatted) {
        // normally you might inspect the string here
        free(formatted);
    }

    // Parse a JSON text
    const char *json_text = "{\"parsed\":true,\"num\":3.14}";
    cJSON *parsed = cJSON_Parse(json_text);
    if (parsed) {
        // Read a value from parsed object
        cJSON *num_item = cJSON_GetObjectItemCaseSensitive(parsed, "num");
        if (num_item && cJSON_IsNumber(num_item)) {
            double val = num_item->valuedouble;
            (void)val; // use val if needed
        }
    }

    // Replace an existing item in the duplicate (change "value" from 42 to 100)
    cJSON_ReplaceItemInObject(dup, "value", cJSON_CreateNumber(100));

    // Detach the array from root and then reattach it under a different key
    cJSON *detached = cJSON_DetachItemFromObject(root, "arr");
    if (detached) {
        cJSON_AddItemToObject(root, "reattached_arr", detached);
    }

    // Replace the first element in the reattached array (if present)
    cJSON *reattached = cJSON_GetObjectItemCaseSensitive(root, "reattached_arr");
    if (reattached && cJSON_IsArray(reattached)) {
        cJSON_ReplaceItemInArray(reattached, 0, cJSON_CreateNumber(10));
    }

    // Clean up: deleting root will delete its children (including reattached array).
    cJSON_Delete(root);

    // dup and parsed are independent and must be deleted separately
    cJSON_Delete(dup);
    cJSON_Delete(parsed);

    return 66;
}