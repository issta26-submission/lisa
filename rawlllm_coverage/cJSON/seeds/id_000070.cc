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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic types
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 42);
    // Add a boolean using CreateTrue (common cJSON helper)
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    // Create an array and add numeric elements
    cJSON *scores = cJSON_CreateArray();
    if (!scores) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "scores", scores);

    // Create a nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return -1; }
    cJSON_AddStringToObject(address, "city", "NYC");
    cJSON_AddStringToObject(address, "street", "5th Ave");
    cJSON_AddItemToObject(root, "address", address);

    // Print an unformatted representation (produces a malloc'd string)
    char *unformatted = cJSON_PrintUnformatted(root);
    if (!unformatted) {
        cJSON_Delete(root);
        return -1;
    }

    // Parse the unformatted string back into a new cJSON structure
    cJSON *parsed = cJSON_Parse(unformatted);
    if (!parsed) {
        free(unformatted);
        cJSON_Delete(root);
        return -1;
    }

    // Access some items from the parsed object (read-only checks)
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!(name_item && cJSON_IsString(name_item))) {
        // still continue but note something unexpected
    }
    cJSON *age_item = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!(age_item && cJSON_IsNumber(age_item))) {
        // unexpected, but proceed for demonstration
    }

    // Duplicate the parsed structure (deep duplicate)
    cJSON *dup = cJSON_Duplicate(parsed, 1); // 1 == recurse
    if (!dup) {
        cJSON_Delete(parsed);
        free(unformatted);
        cJSON_Delete(root);
        return -1;
    }

    // Modify the duplicate in-place: update the age value and add a new field
    cJSON *age_dup = cJSON_GetObjectItemCaseSensitive(dup, "age");
    if (age_dup && cJSON_IsNumber(age_dup)) {
        age_dup->valuedouble = 43;
        age_dup->valueint = 43;
    }
    cJSON_AddStringToObject(dup, "note", "modified copy");

    // Work with the array in the duplicate: check size and first element
    cJSON *scores_dup_item = cJSON_GetObjectItemCaseSensitive(dup, "scores");
    if (scores_dup_item && cJSON_IsArray(scores_dup_item)) {
        int count = cJSON_GetArraySize(scores_dup_item);
        if (count > 0) {
            cJSON *first = cJSON_GetArrayItem(scores_dup_item, 0);
            if (first && cJSON_IsNumber(first)) {
                // For demonstration, create a copy of the array and attach it
                cJSON *scores_copy = cJSON_Duplicate(scores_dup_item, 1);
                if (scores_copy) {
                    cJSON_AddItemToObject(dup, "scores_copy", scores_copy);
                }
            }
        }
    }

    // Print formatted version of the modified duplicate
    char *formatted = cJSON_Print(dup);
    if (!formatted) {
        // cleanup and return error
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        free(unformatted);
        cJSON_Delete(root);
        return -1;
    }

    // Clean up allocated strings and cJSON objects
    free(formatted);
    free(unformatted);

    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reach here, the sequence of API usages succeeded
    return 66;
}