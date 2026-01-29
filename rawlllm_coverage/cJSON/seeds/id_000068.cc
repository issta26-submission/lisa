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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Demonstrate a realistic sequence of cJSON API usage.
    // Note: this function assumes the cJSON header is included and linked in the build environment.
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 1;
    }

    // Add basic types
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("John"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "married", cJSON_CreateTrue());

    // Create an array and add items
    cJSON *children = cJSON_CreateArray();
    if (!children) {
        cJSON_Delete(root);
        return 2;
    }
    cJSON_AddItemToArray(children, cJSON_CreateString("Anna"));
    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));
    cJSON_AddItemToObject(root, "children", children);

    // Print unformatted JSON (returns malloc'd string)
    char *out = cJSON_PrintUnformatted(root);
    if (!out) {
        cJSON_Delete(root);
        return 3;
    }
    // For this test we don't need the string, just free it
    free(out);

    // Duplicate the whole structure (deep copy)
    cJSON *dup = cJSON_Duplicate(root, 1);
    if (!dup) {
        cJSON_Delete(root);
        return 4;
    }

    // Verify duplication by checking a field
    cJSON *dup_name = cJSON_GetObjectItemCaseSensitive(dup, "name");
    if (!dup_name || !cJSON_IsString(dup_name) || strcmp(dup_name->valuestring, "John") != 0) {
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return 5;
    }

    // Replace an item in the original object
    cJSON_ReplaceItemInObject(root, "age", cJSON_CreateNumber(31));

    // Detach the first child from the children array and delete it separately
    cJSON *detached = cJSON_DetachItemFromArray(children, 0);
    if (detached) {
        cJSON_Delete(detached);
    }
    // Check array size after detach
    int remaining = cJSON_GetArraySize(children);
    (void)remaining; // avoid unused-variable warning in some builds

    // Parse a JSON string and add the parsed object into root
    const char *sample = "{\"parsed_key\":123, \"flag\":false}";
    cJSON *parsed = cJSON_Parse(sample);
    if (!parsed) {
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return 6;
    }
    // cJSON_AddItemToObject takes ownership of parsed
    cJSON_AddItemToObject(root, "extra", parsed);

    // Make a shallow duplicate of the parsed object (non-recursive)
    cJSON *shallow = cJSON_Duplicate(parsed, 0);
    if (!shallow) {
        cJSON_Delete(dup);
        cJSON_Delete(root); // this will also delete parsed (as it was added to root)
        return 7;
    }
    cJSON_Delete(shallow);

    // Print pretty JSON and free the returned string
    char *pretty = cJSON_Print(root);
    if (pretty) {
        free(pretty);
    }

    // Access an item inside the parsed object we added as "extra"
    cJSON *extra = cJSON_GetObjectItemCaseSensitive(root, "extra");
    if (extra) {
        cJSON *flag = cJSON_GetObjectItemCaseSensitive(extra, "flag");
        if (flag) {
            // intentionally do nothing; just demonstrate retrieval
            (void)flag;
        }
    }

    // Clean up: delete duplicated copy and the original root (which deletes all its children)
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // If we reach here, the API sequence succeeded
    return 66;
}