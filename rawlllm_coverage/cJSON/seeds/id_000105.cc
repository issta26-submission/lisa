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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = nullptr;
    cJSON *parsed = nullptr;
    cJSON *dup = nullptr;
    char *printed = nullptr;

    // Create root object and add basic members
    root = cJSON_CreateObject();
    if (root) {
        cJSON_AddStringToObject(root, "name", "cJSON");
        cJSON_AddNumberToObject(root, "version", 1.7);

        // Create an array and add some numbers
        cJSON *primes = cJSON_CreateArray();
        if (primes) {
            cJSON_AddItemToArray(primes, cJSON_CreateNumber(2));
            cJSON_AddItemToArray(primes, cJSON_CreateNumber(3));
            cJSON_AddItemToArray(primes, cJSON_CreateNumber(5));
            cJSON_AddItemToArray(primes, cJSON_CreateNumber(7));
            cJSON_AddItemToObject(root, "primes", primes);
        }

        // Create a nested object with a boolean and a null
        cJSON *nested = cJSON_CreateObject();
        if (nested) {
            cJSON_AddItemToObject(nested, "note", cJSON_CreateNull());
            cJSON_AddItemToObject(nested, "active", cJSON_CreateTrue());
            cJSON_AddItemToObject(root, "nested", nested);
        }
    }

    // Print the JSON to a string
    printed = cJSON_Print(root); // allocated string (malloc)
    if (printed) {
        // Parse the printed string back into a cJSON structure
        parsed = cJSON_Parse(printed);
    }

    // Inspect the parsed JSON
    if (parsed) {
        cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (name_item && cJSON_IsString(name_item) && name_item->valuestring) {
            // name_item->valuestring is available
        }

        cJSON *primes_item = cJSON_GetObjectItemCaseSensitive(parsed, "primes");
        if (primes_item && cJSON_IsArray(primes_item)) {
            int count = cJSON_GetArraySize(primes_item);
            for (int i = 0; i < count; ++i) {
                cJSON *elem = cJSON_GetArrayItem(primes_item, i);
                if (elem && cJSON_IsNumber(elem)) {
                    double v = elem->valuedouble;
                    (void)v; // use value if needed
                }
            }
        }

        // Duplicate the parsed JSON (deep copy)
        dup = cJSON_Duplicate(parsed, 1);
        if (dup) {
            // Replace the "version" in the duplicate
            cJSON_ReplaceItemInObject(dup, "version", cJSON_CreateNumber(2.0));

            // Detach the "primes" array from the duplicate (removes from dup, returns pointer)
            cJSON *detached = cJSON_DetachItemFromObject(dup, "primes");
            if (detached) {
                // Re-attach the detached array under a new key in the original parsed object
                cJSON_AddItemToObject(parsed, "primes_detached", detached);
            }
        }
    }

    // Cleanup: free printed string and delete cJSON structures
    if (printed) {
        free(printed);
        printed = nullptr;
    }
    if (root) {
        cJSON_Delete(root);
        root = nullptr;
    }
    if (parsed) {
        cJSON_Delete(parsed);
        parsed = nullptr;
    }
    if (dup) {
        cJSON_Delete(dup);
        dup = nullptr;
    }

    // Final required return value
    return 66;
}