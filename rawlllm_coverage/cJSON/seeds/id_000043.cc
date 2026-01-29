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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API usages:
    // - create objects/arrays/values
    // - add, replace, detach items
    // - duplicate, print, parse
    // - proper deletion/cleanup
    //
    // Note: This code assumes the cJSON header is included in the translation unit
    // (e.g. #include "cJSON.h") and the program links against cJSON. It also
    // assumes the runtime provides free() and related libc functions.

    cJSON *root = NULL;
    cJSON *skills = NULL;
    cJSON *nested = NULL;
    cJSON *dup = NULL;
    cJSON *parsed = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    char *unprinted = NULL;
    char *dup_printed = NULL;

    // 1) Create root object and add basic items
    root = cJSON_CreateObject();
    if (!root) goto error;

    if (!cJSON_AddItemToObject(root, "name", cJSON_CreateString("ChatGPT"))) goto error;
    // helper to add number
    if (!cJSON_AddNumberToObject(root, "age", 3)) goto error;
    // boolean true
    if (!cJSON_AddItemToObject(root, "active", cJSON_CreateTrue())) goto error;

    // 2) Create and populate an array, then add to root
    skills = cJSON_CreateArray();
    if (!skills) goto error;
    if (!cJSON_AddItemToArray(skills, cJSON_CreateString("C++"))) goto error;
    if (!cJSON_AddItemToArray(skills, cJSON_CreateString("JSON"))) goto error;
    if (!cJSON_AddItemToArray(skills, cJSON_CreateString("cJSON"))) goto error;
    cJSON_AddItemToObject(root, "skills", skills); // ownership transferred to root
    skills = NULL; // don't delete separately

    // 3) Add a nested object
    nested = cJSON_CreateObject();
    if (!nested) goto error;
    if (!cJSON_AddNumberToObject(nested, "level", 5)) goto error;
    cJSON_AddItemToObject(root, "nested", nested);
    nested = NULL;

    // 4) Print formatted and unformatted JSON (allocate strings)
    printed = cJSON_Print(root); // pretty
    unprinted = cJSON_PrintUnformatted(root); // compact
    if (!printed || !unprinted) goto error;

    // 5) Duplicate the entire structure (deep copy)
    dup = cJSON_Duplicate(root, 1);
    if (!dup) goto error;
    dup_printed = cJSON_Print(dup);
    if (!dup_printed) goto error;

    // 6) Manipulate the duplicate: replace the age, detach skills and reuse it
    {
        // replace "age" with 4
        cJSON *new_age = cJSON_CreateNumber(4);
        if (!new_age) goto error;
        if (cJSON_ReplaceItemInObject(dup, "age", new_age) == 0) {
            // Replace succeeded (returns void in some versions; but commonly available)
        }

        // detach "skills" from duplicate (removes from dup but returns pointer)
        detached = cJSON_DetachItemFromObject(dup, "skills");
        if (!detached) {
            // if detach failed, continue but don't try to reattach
        } else {
            // attach detached array onto original root under a new key
            cJSON_AddItemToObject(root, "skills_detached_from_dup", detached);
            detached = NULL; // ownership transferred to root
        }
    }

    // 7) Parse the unformatted string back into a cJSON structure
    parsed = cJSON_Parse(unprinted);
    if (!parsed) goto error;

    // 8) Inspect and replace an item in parsed
    {
        cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (cJSON_IsString(nameItem) && (nameItem->valuestring != NULL)) {
            // replace the name value with a new string
            if (!cJSON_ReplaceItemInObject(parsed, "name", cJSON_CreateString("Assistant"))) {
                // if ReplaceItemInObject not available as non-void, code continues
            }
        }
    }

    // 9) Detach nested and explicitly delete it
    {
        cJSON *det = cJSON_DetachItemFromObject(parsed, "nested");
        if (det) {
            cJSON_Delete(det);
            det = NULL;
        }
    }

    // 10) Cleanup: free printed strings and delete cJSON trees
    if (printed) { free(printed); printed = NULL; }
    if (unprinted) { free(unprinted); unprinted = NULL; }
    if (dup_printed) { free(dup_printed); dup_printed = NULL; }

    if (root) { cJSON_Delete(root); root = NULL; }
    if (dup) { cJSON_Delete(dup); dup = NULL; }
    if (parsed) { cJSON_Delete(parsed); parsed = NULL; }

    // If we reached here, operations succeeded
    return 66;

error:
    // best-effort cleanup on error
    if (printed) { free(printed); printed = NULL; }
    if (unprinted) { free(unprinted); unprinted = NULL; }
    if (dup_printed) { free(dup_printed); dup_printed = NULL; }

    if (skills) { cJSON_Delete(skills); skills = NULL; }
    if (nested) { cJSON_Delete(nested); nested = NULL; }
    if (detached) { cJSON_Delete(detached); detached = NULL; }
    if (root) { cJSON_Delete(root); root = NULL; }
    if (dup) { cJSON_Delete(dup); dup = NULL; }
    if (parsed) { cJSON_Delete(parsed); parsed = NULL; }

    return 0;
}