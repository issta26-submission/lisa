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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of cJSON API usages:
    // create objects/arrays, add/replace/detach items, (un)formatted printing,
    // parsing, duplicating, minifying, error checking and cleanup.
    //
    // It returns 66 on success; other non-66 return values indicate failures
    // at particular steps (useful for debugging).

    cJSON *root = nullptr;
    cJSON *parsed = nullptr;
    cJSON *dup = nullptr;
    cJSON *badparse = nullptr;

    // 1) Create root object and populate it with basic values.
    root = cJSON_CreateObject();
    if (!root) return 1;
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);

    // 2) Create a nested object and add it to root.
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 2; }
    cJSON_AddStringToObject(address, "city", "NYC");
    cJSON_AddStringToObject(address, "street", "Main St");
    cJSON_AddItemToObject(root, "address", address); // address owned by root now

    // 3) Create an array and add items to it, then attach to root.
    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); return 3; }
    cJSON_AddItemToArray(items, cJSON_CreateString("one"));
    cJSON_AddItemToArray(items, cJSON_CreateString("two"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "items", items); // items owned by root

    // 4) Print unformatted JSON and parse it back.
    char *unformatted = cJSON_PrintUnformatted(root);
    if (!unformatted) { cJSON_Delete(root); return 4; }
    parsed = cJSON_Parse(unformatted);
    free(unformatted); // cJSON_PrintUnformatted uses malloc/free
    if (!parsed) { cJSON_Delete(root); return 5; }

    // 5) Basic verification on parsed data.
    cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!pname || !cJSON_IsString(pname) || !pname->valuestring
        || strcmp(pname->valuestring, "example") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 6;
    }

    cJSON *pitems = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!pitems || !cJSON_IsArray(pitems) || cJSON_GetArraySize(pitems) != 3) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 7;
    }

    // 6) Duplicate the original root (deep copy) and then modify the duplicate.
    dup = cJSON_Duplicate(root, /*recurse=*/1);
    if (!dup) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 8;
    }

    // Detach the address object from dup (so we own it) and extend it.
    cJSON *detached_address = cJSON_DetachItemFromObject(dup, "address");
    if (!detached_address) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 9;
    }
    // Add another field to detached_address and reattach under a different key.
    cJSON_AddStringToObject(detached_address, "country", "USA");
    cJSON_AddItemToObject(dup, "location", detached_address); // ownership transferred to dup

    // Replace the "name" field in dup with a new value.
    cJSON_ReplaceItemInObject(dup, "name", cJSON_CreateString("newname"));

    // 7) Array manipulations on dup: detach the first array element, then insert it back.
    cJSON *dup_items = cJSON_GetObjectItemCaseSensitive(dup, "items");
    if (dup_items && cJSON_IsArray(dup_items)) {
        if (cJSON_GetArraySize(dup_items) > 0) {
            cJSON *first = cJSON_DetachItemFromArray(dup_items, 0);
            if (first) {
                // Insert back at index 0 to demonstrate insertion API.
                cJSON_InsertItemInArray(dup_items, 0, first);
            }
        }
    }

    // 8) Pretty-print the duplicate, make a mutable copy, minify it, then free.
    char *pretty = cJSON_Print(dup); // formatted (pretty) output
    if (pretty) {
        size_t len = strlen(pretty);
        char *copy = (char*)malloc(len + 1);
        if (copy) {
            strcpy(copy, pretty);
            cJSON_Minify(copy); // remove whitespace in-place
            free(copy);
        }
        free(pretty);
    }

    // 9) Parse an invalid JSON to exercise cJSON_GetErrorPtr().
    const char *bad_json = "{\"incomplete\": [1,2,}";
    badparse = cJSON_Parse(bad_json);
    const char *errptr = nullptr;
    if (!badparse) {
        errptr = cJSON_GetErrorPtr(); // may be non-null and point into bad_json
        (void)errptr; // silence unused-variable warnings in contexts that don't print
    } else {
        cJSON_Delete(badparse);
        badparse = nullptr;
    }

    // 10) Cleanup all created objects.
    if (dup) cJSON_Delete(dup);
    if (parsed) cJSON_Delete(parsed);
    if (root) cJSON_Delete(root);
    // badparse already handled above

    // If we reached here, the sequence executed successfully.
    return 66;
}