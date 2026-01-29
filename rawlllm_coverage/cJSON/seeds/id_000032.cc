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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Include headers here so this function is self-contained when pasted into a file.
    // (Preprocessor directives are allowed inside functions.)
    #include <cstdlib>   // free
    #include <cstring>   // strcmp (not strictly necessary here)
    #include <string>    // std::string
    #include "cJSON.h"

    cJSON *root = nullptr;
    cJSON *arr = nullptr;
    char *printed = nullptr;
    cJSON *dup = nullptr;
    cJSON *parsed = nullptr;
    cJSON *parsed_min = nullptr;
    cJSON *detached = nullptr;

    // 1) Create a JSON object and populate with various types
    root = cJSON_CreateObject();
    if (!root) goto fail;

    // Add string and number using convenience macros
    if (!cJSON_AddStringToObject(root, "name", "example")) goto fail;
    if (!cJSON_AddNumberToObject(root, "value", 42)) goto fail;

    // Add an array with mixed contents
    arr = cJSON_CreateArray();
    if (!arr) goto fail;
    cJSON_AddItemToObject(root, "items", arr); // arr now owned by root
    if (!cJSON_AddItemToArray(arr, cJSON_CreateNumber(1))) goto fail;
    if (!cJSON_AddItemToArray(arr, cJSON_CreateString("two"))) goto fail;
    if (!cJSON_AddItemToArray(arr, cJSON_CreateTrue())) goto fail; // push boolean

    // Add explicit boolean and null members
    cJSON_AddItemToObject(root, "valid", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "missing", cJSON_CreateNull());

    // 2) Print (pretty) the JSON to a string
    printed = cJSON_Print(root);
    if (!printed) goto fail;

    // 3) Duplicate the root (deep copy)
    dup = cJSON_Duplicate(root, 1); // recurse = 1 (deep copy)
    if (!dup) goto fail;

    // 4) Modify the duplicate: replace the "value" number
    // Use delete + add to avoid relying on replace API availability
    cJSON_DeleteItemFromObject(dup, "value");
    if (!cJSON_AddNumberToObject(dup, "value", 100)) goto fail;

    // 5) Parse the printed string back into a cJSON structure
    parsed = cJSON_Parse(printed);
    if (!parsed) goto fail;

    // 6) Inspect a parsed item (ensure types are as expected)
    {
        cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (!name_item || !cJSON_IsString(name_item) || !name_item->valuestring) goto fail;
        // (Could compare string content if desired)
    }

    // 7) Demonstrate minify: make a modifiable copy using std::string
    {
        std::string mincopy = printed; // copy the pretty printed JSON
        // cJSON_Minify operates in-place on a char*
        cJSON_Minify(&mincopy[0]);
        parsed_min = cJSON_Parse(mincopy.c_str());
        if (!parsed_min) goto fail;
    }

    // 8) Detach an item from the original array (removes it from the array but does not free it)
    detached = cJSON_DetachItemFromArray(arr, 0);
    if (detached) {
        // We own detached now; free it explicitly
        cJSON_Delete(detached);
        detached = nullptr;
    }

    // All operations succeeded. Clean up and return success code 66.
    cJSON_Delete(parsed_min); parsed_min = nullptr;
    cJSON_Delete(parsed); parsed = nullptr;
    cJSON_Delete(dup); dup = nullptr;
    free(printed); printed = nullptr;
    cJSON_Delete(root); root = nullptr;

    return 66;

fail:
    // best-effort cleanup on failure
    if (printed) free(printed);
    if (parsed_min) cJSON_Delete(parsed_min);
    if (parsed) cJSON_Delete(parsed);
    if (dup) cJSON_Delete(dup);
    if (detached) cJSON_Delete(detached);
    if (root) cJSON_Delete(root);

    return -1;
}