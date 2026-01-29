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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup = NULL;
    cJSON *parsed = NULL;
    char *printed = NULL;
    char *pretty = NULL;
    char *minify_buf = NULL;
    cJSON *detached = NULL;
    int success = 0;

    /* Create a root object and populate it with various types */
    root = cJSON_CreateObject();
    if (!root) goto cleanup;

    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddItemToObject(root, "value", cJSON_CreateNumber(42.5));
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    /* Create an array and add items */
    arr = cJSON_CreateArray();
    if (!arr) goto cleanup;
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));
    cJSON_AddItemToObject(root, "items", arr); /* arr is now owned by root */

    /* Print unformatted JSON */
    printed = cJSON_PrintUnformatted(root);
    if (!printed) goto cleanup;

    /* Duplicate the whole structure (deep copy) */
    dup = cJSON_Duplicate(root, 1); /* 1 -> recurse */
    if (!dup) goto cleanup;

    /* Parse the string we printed back into a cJSON structure */
    parsed = cJSON_Parse(printed);
    if (!parsed) goto cleanup;

    /* Inspect parsed contents */
    {
        cJSON *gotName = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (!cJSON_IsString(gotName)) goto cleanup;

        cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
        if (!cJSON_IsArray(items)) goto cleanup;

        /* get size and first item */
        int count = cJSON_GetArraySize(items);
        (void)count;
        cJSON *first = cJSON_GetArrayItem(items, 0);
        if (!first) goto cleanup;
    }

    /* Replace a value in the duplicated object */
    cJSON_ReplaceItemInObject(dup, "value", cJSON_CreateNumber(99));

    /* Detach the first item from the original array and move it into dup */
    detached = cJSON_DetachItemFromArray(arr, 0);
    if (detached) {
        /* transfer ownership to dup under a new key */
        cJSON_AddItemToObject(dup, "moved_item", detached);
        detached = NULL; /* ownership transferred; avoid double-delete */
    }

    /* Detach a named item from root and reattach it to dup under a different name */
    {
        cJSON *detachedName = cJSON_DetachItemFromObject(root, "name");
        if (detachedName) {
            cJSON_AddItemToObject(dup, "detached_name", detachedName);
            /* detachedName now owned by dup */
        }
    }

    /* Replace the second element in dup's array with a string */
    {
        cJSON *dup_items = cJSON_GetObjectItemCaseSensitive(dup, "items");
        if (dup_items && cJSON_IsArray(dup_items)) {
            cJSON_ReplaceItemInArray(dup_items, 1, cJSON_CreateString("TWO"));
        }
    }

    /* Pretty-print the duplicated structure, then minify a copy of the pretty output */
    pretty = cJSON_Print(dup);
    if (pretty) {
        /* allocate buffer using cJSON allocator and copy manually (no external headers required) */
        unsigned int len = 0;
        while (pretty[len]) ++len;
        minify_buf = (char*)cJSON_malloc((size_t)len + 1);
        if (minify_buf) {
            for (unsigned int i = 0; i <= len; ++i) minify_buf[i] = pretty[i];
            cJSON_Minify(minify_buf); /* minify in place */
            /* free the minified buffer using cJSON's free */
            cJSON_free(minify_buf);
            minify_buf = NULL;
        }
    }

    /* All operations succeeded */
    success = 1;

cleanup:
    /* If something was detached and not reattached, delete it */
    if (detached) cJSON_Delete(detached);

    /* Free parsed/created structures. Note: items moved between objects should not be double-deleted. */
    if (parsed) cJSON_Delete(parsed);
    if (root) cJSON_Delete(root);
    if (dup) cJSON_Delete(dup);

    /* Free printed strings produced by cJSON_Print / cJSON_PrintUnformatted using cJSON_free */
    if (printed) cJSON_free(printed);
    if (pretty) cJSON_free(pretty);

    return success ? 66 : 0;
}