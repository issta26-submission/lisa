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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Use a sequence of common cJSON APIs to create, manipulate, serialize,
    // parse, duplicate and delete JSON structures.
    //
    // The function returns 66 on success, and a non-66 negative value on failure.
    // (In real tests you might want more granular error codes.)

    cJSON *root = nullptr;
    cJSON *features = nullptr;
    cJSON *dup = nullptr;
    cJSON *parsed = nullptr;
    cJSON *parsed_dup_shallow = nullptr;
    char *printed = nullptr;
    char *printed_un = nullptr;
    char *minify_buf = nullptr;

    // 1) Create root object and populate it
    root = cJSON_CreateObject();
    if (!root) return -1;

    // Add simple items
    cJSON_AddItemToObject(root, "project", cJSON_CreateString("cJSON"));
    cJSON_AddItemToObject(root, "version", cJSON_CreateNumber(1.7));
    cJSON_AddItemToObject(root, "stable", cJSON_CreateTrue());

    // Create an array and add it to root
    features = cJSON_CreateArray();
    if (!features) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(features, cJSON_CreateString("parsing"));
    cJSON_AddItemToArray(features, cJSON_CreateString("printing"));
    cJSON_AddItemToArray(features, cJSON_CreateString("manipulation"));
    cJSON_AddItemToObject(root, "features", features);

    // 2) Duplicate (deep copy) the root object and modify the duplicate
    dup = cJSON_Duplicate(root, true); // deep copy
    if (!dup) { cJSON_Delete(root); return -3; }
    // Replace the "version" in the duplicate with a new number
    cJSON_ReplaceItemInObject(dup, "version", cJSON_CreateNumber(2.0));

    // 3) Serialize both original and duplicate (pretty and unformatted)
    printed = cJSON_Print(root);
    printed_un = cJSON_PrintUnformatted(dup);
    if (!printed || !printed_un) {
        if (printed) free(printed);
        if (printed_un) free(printed_un);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -4;
    }

    // 4) Demonstrate minify (in-place) and parsing the minified string
    // Make a writable copy of the unformatted string for minification
    minify_buf = (char*)malloc(strlen(printed_un) + 1);
    if (!minify_buf) {
        free(printed);
        free(printed_un);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -5;
    }
    strcpy(minify_buf, printed_un);
    cJSON_Minify(minify_buf);           // removes whitespace/comments in-place
    parsed = cJSON_Parse(minify_buf);   // parse minified text
    if (!parsed) {
        free(printed);
        free(printed_un);
        free(minify_buf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return -6;
    }

    // 5) Manipulate arrays and objects: detach first feature and reattach elsewhere
    cJSON *features_arr = cJSON_GetObjectItem(root, "features");
    if (!features_arr) {
        // cleanup and fail if unexpected structure
        free(printed);
        free(printed_un);
        free(minify_buf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        return -7;
    }

    // Detach the first array element and add it to root under a different key
    cJSON *detached = cJSON_DetachItemFromArray(features_arr, 0);
    if (!detached) {
        free(printed);
        free(printed_un);
        free(minify_buf);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        return -8;
    }
    cJSON_AddItemToObject(root, "first_feature", detached);

    // Replace the second element of the features array (index 1) with a number
    // (Note: after detaching index 0, original index 1 becomes new index 0; we check size lightly)
    // If the array still has at least one element, replace item at index 1 (if present), else replace index 0
    int replace_index = 1;
    if (cJSON_GetArraySize(features_arr) <= 1) replace_index = 0;
    cJSON_ReplaceItemInArray(features_arr, replace_index, cJSON_CreateNumber(42));

    // 6) Demonstrate shallow duplication of a parsed object
    parsed_dup_shallow = cJSON_Duplicate(parsed, false); // shallow copy

    // At this point we have exercised:
    // - create, add, arrays
    // - duplicate (deep and shallow)
    // - replace, detach, replace in array
    // - print (pretty/unformatted)
    // - minify and parse
    // - delete/cleanup

    // Cleanup: free printed strings and delete all cJSON allocations
    free(printed);
    free(printed_un);
    free(minify_buf);

    cJSON_Delete(root);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(parsed_dup_shallow);

    // Success
    return 66;
}