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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function exercises a broad sequence of cJSON APIs.
    // It builds objects/arrays, prints/parses JSON, duplicates, detaches,
    // replaces items, minifies a string, and cleans up. Returns 66 on success.

    // Create root object and populate with various types
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Alice"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "is_student", cJSON_CreateFalse());

    // Create an array of scores and attach to root
    cJSON *scores = cJSON_CreateArray();
    if (!scores) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(85));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(92));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(78));
    cJSON_AddItemToObject(root, "scores", scores);

    // Create a nested object for address
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 3; }
    cJSON_AddStringToObject(address, "city", "Wonderland");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Render to compact (unformatted) JSON string
    char *out = cJSON_PrintUnformatted(root);
    if (!out) { cJSON_Delete(root); return 4; }

    // Parse the printed JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(out);
    if (!parsed) { free(out); cJSON_Delete(root); return 5; }

    // Validate some parsed values
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL) || strcmp(name->valuestring, "Alice") != 0) {
        free(out); cJSON_Delete(root); cJSON_Delete(parsed); return 6;
    }

    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age) || age->valuedouble != 30.0) {
        free(out); cJSON_Delete(root); cJSON_Delete(parsed); return 7;
    }

    cJSON *scores_parsed = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    if (!cJSON_IsArray(scores_parsed) || cJSON_GetArraySize(scores_parsed) != 3) {
        free(out); cJSON_Delete(root); cJSON_Delete(parsed); return 8;
    }

    // Duplicate the parsed tree (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) { free(out); cJSON_Delete(root); cJSON_Delete(parsed); return 9; }

    // Detach the "scores" array from the duplicate and manipulate it separately
    cJSON *detached_scores = cJSON_DetachItemFromObject(dup, "scores");
    if (!detached_scores) {
        free(out); cJSON_Delete(root); cJSON_Delete(parsed); cJSON_Delete(dup); return 10;
    }
    // Add an extra score to the detached array
    cJSON_AddItemToArray(detached_scores, cJSON_CreateNumber(100));
    // Reattach under a different key in dup
    cJSON_AddItemToObject(dup, "extra_scores", detached_scores);

    // Replace the second element in the original root's scores array (index 1) with a new number
    cJSON *scores_root = cJSON_GetObjectItemCaseSensitive(root, "scores");
    if (!scores_root || !cJSON_IsArray(scores_root)) {
        free(out); cJSON_Delete(root); cJSON_Delete(parsed); cJSON_Delete(dup); return 11;
    }
    // Replace item at index 1
    cJSON_ReplaceItemInArray(scores_root, 1, cJSON_CreateNumber(95));

    // Detach and delete the address from the parsed tree to demonstrate detach/delete workflow
    cJSON *addr_detached = cJSON_DetachItemFromObject(parsed, "address");
    if (addr_detached) {
        cJSON_Delete(addr_detached);
    }

    // Delete an item directly from the root (demonstrate DeleteItemFromObject)
    cJSON_DeleteItemFromObject(root, "is_student");

    // Demonstrate minify: create a copy of the printed JSON and minify it in-place
    char *minify_buf = NULL;
#ifdef _MSC_VER
    // strdup may be named _strdup on MSVC
    minify_buf = _strdup(out);
#else
    minify_buf = strdup(out);
#endif
    if (minify_buf) {
        cJSON_Minify(minify_buf);
        // Parse the minified JSON to ensure it's valid
        cJSON *minparsed = cJSON_Parse(minify_buf);
        if (minparsed) {
            cJSON_Delete(minparsed);
        } else {
            // If parsing minified string fails, keep going but note cleanup below
        }
    }

    // Clean up all allocated resources
    free(out);
    if (minify_buf) free(minify_buf);

    cJSON_Delete(root);     // deletes scores and address attached to root (address already detached earlier)
    cJSON_Delete(parsed);
    cJSON_Delete(dup);      // deletes the reattached extra_scores inside dup

    // If we reached here without returning an error code, return 66 to indicate success.
    return 66;
}