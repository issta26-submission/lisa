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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    /* Include headers locally to make this function self-contained for the
       purpose of this example. In real projects, put includes at top of file. */
    #include <cstdlib>
    #include "cJSON.h"

    int result = 66; /* default success return value required by the task */

    /* 1) Create a root object and populate with various types */
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0; /* allocation failure */

    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 123.45);
    /* create true and null items using cJSON helpers */
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nothing", cJSON_CreateNull());

    /* 2) Create an array and add mixed items */
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));
    /* attach array to root */
    cJSON_AddItemToObject(root, "items", arr);

    /* 3) Create a nested object */
    cJSON *child = cJSON_CreateObject();
    if (!child) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(child, "child_name", "inner");
    cJSON_AddNumberToObject(child, "child_value", 7);
    cJSON_AddItemToObject(root, "child", child);

    /* 4) Create a string array using helper (if available) */
    const char *strings[] = { "alpha", "beta", "gamma" };
    /* Some cJSON builds provide cJSON_CreateStringArray */
#ifdef cJSON_CreateStringArray
    cJSON *sarr = cJSON_CreateStringArray(strings, 3);
    if (sarr) cJSON_AddItemToObject(root, "strings", sarr);
#endif

    /* 5) Serialize the JSON (pretty and unformatted) */
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    /* We won't require these prints to be non-null for success, but check them */
    if (!pretty || !compact) {
        /* Clean up and fail if allocations for printing failed */
        if (pretty) std::free(pretty);
        if (compact) std::free(compact);
        cJSON_Delete(root);
        return 0;
    }

    /* 6) Parse the serialized output back to a cJSON structure */
    cJSON *parsed = cJSON_Parse(pretty);
    if (!parsed) {
        std::free(pretty);
        std::free(compact);
        cJSON_Delete(root);
        return 0;
    }

    /* 7) Access members from the parsed object safely */
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (name && cJSON_IsString(name)) {
        /* example read - not used further */
        const char *name_text = name->valuestring;
        (void)name_text;
    }

    cJSON *value = cJSON_GetObjectItemCaseSensitive(parsed, "value");
    if (value && cJSON_IsNumber(value)) {
        double v = value->valuedouble;
        (void)v;
    }

    /* 8) Work with the array: iterate and operate on items */
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (parsed_items && cJSON_IsArray(parsed_items)) {
        int count = cJSON_GetArraySize(parsed_items);
        for (int i = 0; i < count; ++i) {
            cJSON *it = cJSON_GetArrayItem(parsed_items, i);
            if (it) {
                /* Example: if number, read it; if string, read it */
                if (cJSON_IsNumber(it)) {
                    double num = it->valuedouble;
                    (void)num;
                } else if (cJSON_IsString(it)) {
                    const char *s = it->valuestring;
                    (void)s;
                }
            }
        }
    }

    /* 9) Duplicate the parsed JSON (deep copy) */
    cJSON *dup = cJSON_Duplicate(parsed, 1); /* 1 = recurse (deep) */
    if (!dup) {
        std::free(pretty);
        std::free(compact);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    /* 10) Replace an item in the original root (change "value" to 999) */
    cJSON *replacement = cJSON_CreateNumber(999);
    if (replacement) {
        /* Replace in the original root object (not in parsed or dup) */
        cJSON_ReplaceItemInObject(root, "value", replacement);
    }

    /* 11) Detach an item from the array and re-insert it (demonstrate detach/insert) */
    if (arr && cJSON_IsArray(arr) && cJSON_GetArraySize(arr) > 0) {
        cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
        if (detached) {
            /* Insert it again at index 0 */
            cJSON_InsertItemInArray(arr, 0, detached);
        }
    }

    /* 12) Demonstrate removing an object member by name (detach then delete) */
    cJSON *detached_child = cJSON_DetachItemFromObject(root, "child");
    if (detached_child) {
        cJSON_Delete(detached_child); /* fully delete the detached subtree */
    }

    /* 13) Minify a copy of the pretty string (in-place) to show minify API usage */
    char *minify_copy = nullptr;
    if (pretty) {
        minify_copy = (char *)std::malloc(std::strlen(pretty) + 1);
        if (minify_copy) {
            std::strcpy(minify_copy, pretty);
            cJSON_Minify(minify_copy); /* minifies the string in-place */
            /* minified string available in minify_copy if needed */
        }
    }

    /* 14) Cleanup: free prints and delete cJSON structures created here */
    if (pretty) std::free(pretty);
    if (compact) std::free(compact);
    if (minify_copy) std::free(minify_copy);

    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root); /* frees arr and its children too, unless detached */

    /* If we reached here, the sequence was exercised successfully */
    return result;
}