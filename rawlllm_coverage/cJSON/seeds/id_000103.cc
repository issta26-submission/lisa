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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function exercises a variety of cJSON APIs:
    // - create objects/arrays/primitives
    // - add/replace/detach items
    // - duplicate, print, parse, minify
    // - query array/object contents
    // All allocated cJSON structures and printed strings are cleaned up.
    //
    // Note: this function assumes the cJSON header/library is available
    // in the build environment where this function is compiled and linked.

    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_minified = NULL;
    char *printed = NULL;
    char *pretty = NULL;
    int ret = -1; // non-66 indicates failure along the way

    // Create root object
    root = cJSON_CreateObject();
    if (!root) goto cleanup;

    // Add several primitive types to the object
    if (!cJSON_AddItemToObject(root, "name", cJSON_CreateString("Alice"))) goto cleanup;
    if (!cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(30))) goto cleanup;
    if (!cJSON_AddItemToObject(root, "active", cJSON_CreateBool(1))) goto cleanup;
    if (!cJSON_AddItemToObject(root, "note", cJSON_CreateNull())) goto cleanup;

    // Create an array and add items to it
    arr = cJSON_CreateArray();
    if (!arr) goto cleanup;
    if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(10))) goto cleanup;
    if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(20))) goto cleanup;
    if (!cJSON_AddItemToArray(arr, cJSON_CreateString("thirty"))) goto cleanup;
    // Attach array to root
    if (!cJSON_AddItemToObject(root, "values", arr)) goto cleanup;
    // After successful AddItemToObject, 'arr' is owned by 'root' and must not be deleted directly.
    arr = NULL;

    // Duplicate the whole object (deep copy)
    dup = cJSON_Duplicate(root, 1); // deep copy
    if (!dup) goto cleanup;

    // Print (unformatted and pretty) the root object
    printed = cJSON_PrintUnformatted(root);
    if (!printed) goto cleanup;
    pretty = cJSON_Print(root);
    if (!pretty) goto cleanup;

    // Parse the unformatted string back to a cJSON structure
    parsed = cJSON_Parse(printed);
    if (!parsed) goto cleanup;

    // Query some items in the parsed object
    {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
        cJSON *values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
        if (!name || !age || !values) goto cleanup;
        // Check types (use cJSON_IsX macros if available)
        if (!(cJSON_IsString(name) && (name->valuestring != NULL))) goto cleanup;
        if (!cJSON_IsNumber(age)) goto cleanup;
        if (!cJSON_IsArray(values)) goto cleanup;

        // Inspect array contents
        int size = cJSON_GetArraySize(values);
        if (size != 3) goto cleanup;
        cJSON *second = cJSON_GetArrayItem(values, 1);
        if (!second || !cJSON_IsNumber(second)) goto cleanup;
    }

    // Replace the "name" field in the original root
    if (!cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("Bob"))) goto cleanup;

    // Detach the second element of the "values" array and attach it to root as "second"
    {
        cJSON *values_in_root = cJSON_GetObjectItemCaseSensitive(root, "values");
        if (!values_in_root || !cJSON_IsArray(values_in_root)) goto cleanup;
        // Detach item at index 1 (the number 20)
        cJSON *detached = cJSON_DetachItemFromArray(values_in_root, 1);
        if (!detached) goto cleanup;
        // Add detached item to root under key "second"
        if (!cJSON_AddItemToObject(root, "second", detached)) {
            // If add fails, we must delete the detached item
            cJSON_Delete(detached);
            goto cleanup;
        }
        // Detached item now owned by root.
    }

    // Demonstrate minify: create a whitespace-containing JSON string, minify, and parse
    {
        const char *with_ws = "{\n  \"x\" : 123 ,\n  \"y\" : [ 1, 2, 3 ]\n}\n";
        // Create mutable buffer by copying into a std::string-like buffer.
        // Use C allocation here to avoid requiring extra headers in this snippet.
        size_t len = strlen(with_ws);
        char *buf = (char*)malloc(len + 1);
        if (!buf) goto cleanup;
        memcpy(buf, with_ws, len + 1);
        cJSON_Minify(buf); // modifies buf in-place to remove whitespace
        parsed_minified = cJSON_Parse(buf);
        free(buf);
        if (!parsed_minified) goto cleanup;
        // parsed_minified can be deleted later
    }

    // Clean up intermediate structures and strings before final success
    // Note: root still holds the array and newly attached 'second' item.
    // dup, parsed, parsed_minified are separate and must be deleted.
    if (printed) { free(printed); printed = NULL; }
    if (pretty)  { free(pretty); pretty = NULL; }

    cJSON_Delete(parsed);
    parsed = NULL;
    cJSON_Delete(parsed_minified);
    parsed_minified = NULL;
    cJSON_Delete(dup);
    dup = NULL;

    // Finally delete the root (this deletes all items added to root)
    cJSON_Delete(root);
    root = NULL;

    // All cleanup done successfully; indicate success by returning 66
    ret = 66;

cleanup:
    // Defensive cleanup in case of early exit
    if (printed) free(printed);
    if (pretty) free(pretty);
    if (parsed) cJSON_Delete(parsed);
    if (parsed_minified) cJSON_Delete(parsed_minified);
    if (dup) cJSON_Delete(dup);
    if (arr) cJSON_Delete(arr); // only if not already owned by root
    if (root) cJSON_Delete(root);

    return ret;
}