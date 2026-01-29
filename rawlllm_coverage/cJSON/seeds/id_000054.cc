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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a logically correct sequence of cJSON API usage:
    // - create objects/arrays/primitives
    // - add/detach/replace items
    // - print and parse JSON text
    // - duplicate and traverse structures
    // - clean up all allocated cJSON structures and printed string
    //
    // It returns 66 on successful completion as requested.

    cJSON *root = nullptr;
    cJSON *parsed = nullptr;
    cJSON *dup = nullptr;
    char *printed = nullptr;

    // 1) Create root object and add different types of items.
    root = cJSON_CreateObject();
    if (!root) {
        // Allocation failed; nothing to clean up.
        return 66;
    }

    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 29);
    // Add boolean values using provided helpers
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddNullToObject(root, "spouse"); // explicit null value

    // 2) Create a nested object and attach it.
    cJSON *address = cJSON_CreateObject();
    if (address) {
        cJSON_AddStringToObject(address, "city", "NY");
        cJSON_AddStringToObject(address, "street", "5th Ave");
        cJSON_AddItemToObject(root, "address", address); // root owns 'address' now
    }

    // 3) Create an array and add items (strings and a nested object).
    cJSON *languages = cJSON_CreateArray();
    if (languages) {
        cJSON_AddItemToArray(languages, cJSON_CreateString("C++"));
        cJSON_AddItemToArray(languages, cJSON_CreateString("Python"));

        cJSON *meta = cJSON_CreateObject();
        if (meta) {
            cJSON_AddStringToObject(meta, "proficiency", "intermediate");
            cJSON_AddNumberToObject(meta, "years", 6);
            // Add meta object as an element of the array
            cJSON_AddItemToArray(languages, meta);
        }

        cJSON_AddItemToObject(root, "languages", languages); // root owns 'languages'
    }

    // 4) Print unformatted JSON text, then parse it back to a cJSON structure.
    printed = cJSON_PrintUnformatted(root);
    if (printed) {
        parsed = cJSON_Parse(printed);
        // free the printed string as it's no longer needed after parse
        free(printed);
        printed = nullptr;
    }

    // 5) Validate parsed content (basic checks) and duplicate it.
    if (parsed) {
        cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (cJSON_IsString(name_item) && (name_item->valuestring != nullptr)) {
            // name_item->valuestring is available; we don't print here to avoid IO dependency
            (void)name_item->valuestring;
        }

        cJSON *age_item = cJSON_GetObjectItemCaseSensitive(parsed, "age");
        if (cJSON_IsNumber(age_item)) {
            (void)age_item->valuedouble;
        }

        // Duplicate the parsed structure (deep copy)
        dup = cJSON_Duplicate(parsed, /*recurse=*/1);
        // dup now is an independent copy; safe to modify
    }

    // 6) On the duplicate, replace the "age" value and move the "address" from root into dup.
    if (dup) {
        // Replace age with a new number in dup
        cJSON_ReplaceItemInObject(dup, "age", cJSON_CreateNumber(30));

        // Detach "address" from root and reattach it into dup under a different key.
        // Detaching prevents double-free when we delete root later.
        cJSON *detached_address = cJSON_DetachItemFromObject(root, "address");
        if (detached_address) {
            cJSON_AddItemToObject(dup, "moved_address", detached_address);
            // Now dup owns detached_address.
        }
    }

    // 7) Traverse the array inside dup (if present) to show array APIs in use.
    if (dup) {
        cJSON *arr = cJSON_GetObjectItemCaseSensitive(dup, "languages");
        if (arr && cJSON_IsArray(arr)) {
            int count = cJSON_GetArraySize(arr);
            for (int i = 0; i < count; ++i) {
                cJSON *elem = cJSON_GetArrayItem(arr, i);
                if (elem) {
                    if (cJSON_IsString(elem)) {
                        (void)elem->valuestring; // string element
                    } else if (cJSON_IsObject(elem)) {
                        // nested object element; read a field
                        cJSON *years = cJSON_GetObjectItemCaseSensitive(elem, "years");
                        if (cJSON_IsNumber(years)) {
                            (void)years->valuedouble;
                        }
                    }
                }
            }
        }
    }

    // 8) Clean up: delete parsed, dup, and root. Ensure we don't double-delete detached nodes.
    if (parsed) {
        cJSON_Delete(parsed);
        parsed = nullptr;
    }
    if (dup) {
        cJSON_Delete(dup);
        dup = nullptr;
    }
    if (root) {
        cJSON_Delete(root);
        root = nullptr;
    }
    // printed already freed earlier if non-null.

    // If we reach this point, the sequence of API usages completed successfully.
    return 66;
}