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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// A comprehensive sequence of cJSON API usage demonstrating creation, modification,
// duplication, printing, parsing, minifying, array/object manipulation, and cleanup.
int test_cJSON_api_sequence() {
    // Create root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Add basic types using convenience functions and explicit creations
    if (!cJSON_AddStringToObject(root, "name", "Alice")) { cJSON_Delete(root); return 2; }
    if (!cJSON_AddNumberToObject(root, "age", 30)) { cJSON_Delete(root); return 3; }
    if (!cJSON_AddItemToObject(root, "married", cJSON_CreateTrue())) { cJSON_Delete(root); return 4; }
    if (!cJSON_AddItemToObject(root, "has_pet", cJSON_CreateFalse())) { cJSON_Delete(root); return 5; }
    if (!cJSON_AddItemToObject(root, "spouse", cJSON_CreateNull())) { cJSON_Delete(root); return 6; }

    // Create and populate an array
    cJSON *scores = cJSON_CreateArray();
    if (!scores) { cJSON_Delete(root); return 7; }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(95));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(76));
    cJSON_AddItemToObject(root, "scores", scores);

    // Create nested object "address"
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 8; }
    cJSON_AddStringToObject(address, "city", "Wonderland");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Duplicate the whole root (deep duplicate)
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) { cJSON_Delete(root); return 9; }

    // Print duplicate unformatted (compact) and formatted; ensure result strings are freed
    char *dup_unformatted = cJSON_PrintUnformatted(dup);
    if (!dup_unformatted) { cJSON_Delete(dup); cJSON_Delete(root); return 10; }
    free(dup_unformatted);

    char *dup_pretty = cJSON_Print(dup);
    if (!dup_pretty) { cJSON_Delete(dup); cJSON_Delete(root); return 11; }
    // We'll copy the pretty string into a std::string to demonstrate minify (in-place)
    std::string minify_buf(dup_pretty);
    free(dup_pretty);
    // Minify modifies the buffer in-place
    cJSON_Minify(&minify_buf[0]); // safe: &minify_buf[0] gives a non-const writable buffer in C++11+
    // Parse the minified JSON back to a cJSON tree
    cJSON *parsed = cJSON_Parse(minify_buf.c_str());
    if (!parsed) { cJSON_Delete(dup); cJSON_Delete(root); return 12; }

    // Validate some types using type-checking macros
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!name_item || !cJSON_IsString(name_item) || (name_item->valuestring == nullptr)) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return 13;
    }

    // Work with arrays: get size and items
    cJSON *parsed_scores = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    if (!parsed_scores || !cJSON_IsArray(parsed_scores)) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return 14;
    }
    int scores_count = cJSON_GetArraySize(parsed_scores);
    if (scores_count != 3) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return 15;
    }
    // Detach the second element (index 1) and then delete it explicitly
    cJSON *detached_score = cJSON_DetachItemFromArray(parsed_scores, 1);
    if (!detached_score) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return 16;
    }
    cJSON_Delete(detached_score); // free detached element

    // Replace an item in the original root: change "name" to a new string
    if (!cJSON_ReplaceItemInObject(root, "name", cJSON_CreateString("Alice Liddell"))) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return 17;
    }

    // Remove an item from the original root
    cJSON_DeleteItemFromObject(root, "spouse"); // "spouse" was null earlier

    // Demonstrate detach from object: detach address and then delete it
    cJSON *detached_address = cJSON_DetachItemFromObject(root, "address");
    if (!detached_address) {
        cJSON_Delete(parsed); cJSON_Delete(dup); cJSON_Delete(root); return 18;
    }
    cJSON_Delete(detached_address);

    // Finally clean up allocated cJSON structures
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // Success: return the requested value
    return 66;
}