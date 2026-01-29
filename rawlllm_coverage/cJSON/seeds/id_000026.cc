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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of typical cJSON API usages:
    // - create objects/arrays/primitives
    // - add items to objects/arrays
    // - print (serialize) and parse (deserialize)
    // - duplicate, detach, replace, and reattach items
    // - clean up all allocated cJSON structures and serialized buffer
    //
    // Returns 66 on success, non-66 on failure.

    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *arr = NULL;
    cJSON *arrobj = NULL;
    cJSON *parsed = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    int success = 0; // set to 1 on success

    // 1) Create a root object
    root = cJSON_CreateObject();
    if (!root) goto cleanup;

    // 2) Add primitive members
    cJSON_AddStringToObject(root, "name", "cJSON Test");
    cJSON_AddNumberToObject(root, "version", 1.7);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // 3) Create and add a nested object "meta"
    meta = cJSON_CreateObject();
    if (!meta) goto cleanup;
    cJSON_AddStringToObject(meta, "author", "AI");
    cJSON_AddNumberToObject(meta, "count", 42);
    cJSON_AddItemToObject(root, "meta", meta); // ownership transferred to root

    // 4) Create and add an array "items" with mixed contents
    arr = cJSON_CreateArray();
    if (!arr) goto cleanup;
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    arrobj = cJSON_CreateObject();
    if (!arrobj) goto cleanup;
    cJSON_AddStringToObject(arrobj, "key", "value");
    cJSON_AddItemToArray(arr, arrobj);
    cJSON_AddItemToObject(root, "items", arr); // ownership transferred to root

    // 5) Serialize to a compact string
    printed = cJSON_PrintUnformatted(root);
    if (!printed) goto cleanup;

    // 6) Parse back the serialized string
    parsed = cJSON_Parse(printed);
    if (!parsed) goto cleanup;

    // 7) Basic validation of parsed content:
    // - "name" exists and is a string
    // - "version" exists and is approximately 1.7
    {
        cJSON *it_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        cJSON *it_version = cJSON_GetObjectItemCaseSensitive(parsed, "version");
        if (!it_name || !it_name->valuestring) goto cleanup;
        if (!it_version) goto cleanup;
        double diff = it_version->valuedouble - 1.7;
        if ((diff < 0 ? -diff : diff) > 1e-9) goto cleanup;
    }

    // 8) Duplicate the parsed object (deep duplicate)
    dup = cJSON_Duplicate(parsed, 1);
    if (!dup) goto cleanup;

    // 9) Detach the "meta" object from the duplicate and re-add it under a new key
    {
        cJSON *detached_meta = cJSON_DetachItemFromObject(dup, "meta");
        if (!detached_meta) goto cleanup;
        cJSON_AddItemToObject(dup, "meta_copy", detached_meta); // reattach with new name
    }

    // 10) Replace the "name" string in the duplicate
    cJSON_ReplaceItemInObject(dup, "name", cJSON_CreateString("Replaced"));

    // 11) Operate on the duplicate's "items" array: detach index 1 and append it back
    {
        cJSON *dup_items = cJSON_GetObjectItemCaseSensitive(dup, "items");
        if (!dup_items) goto cleanup;
        cJSON *removed = cJSON_DetachItemFromArray(dup_items, 1); // detach the second element
        if (!removed) goto cleanup;
        cJSON_AddItemToArray(dup_items, removed); // append it again
    }

    // If we reached here, sequence completed successfully
    success = 1;

cleanup:
    // Free serialized buffer (use cJSON_free provided by cJSON library)
    if (printed) {
        cJSON_free(printed);
        printed = NULL;
    }

    // Delete cJSON structures (safe to call with NULL)
    if (parsed) {
        cJSON_Delete(parsed);
        parsed = NULL;
    }
    if (root) {
        cJSON_Delete(root);
        root = NULL;
    }
    if (dup) {
        cJSON_Delete(dup);
        dup = NULL;
    }

    if (success) return 66;
    return 0;
}