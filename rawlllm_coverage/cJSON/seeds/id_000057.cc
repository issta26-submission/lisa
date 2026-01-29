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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *parsed = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    char *printed2 = NULL;

    // 1) Create an object and populate it with various item types
    root = cJSON_CreateObject();
    if (!root) goto error;

    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON_AddStringToObject(root, "name", "cJSON test");

    arr = cJSON_CreateArray();
    if (!arr) goto error;
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateString("three"));
    cJSON_AddItemToObject(root, "items", arr); // ownership of arr transferred to root

    cJSON_AddTrueToObject(root, "ok");
    cJSON_AddFalseToObject(root, "failed");
    cJSON_AddNullToObject(root, "nothing");

    // 2) Serialize to a compact string
    printed = cJSON_PrintUnformatted(root);
    if (!printed) goto error;

    // We no longer need the original root after serialization
    cJSON_Delete(root);
    root = NULL;

    // 3) Parse the serialized string back to a cJSON structure
    parsed = cJSON_Parse(printed);
    // free the printed string returned by cJSON_PrintUnformatted
    free(printed);
    printed = NULL;
    if (!parsed) goto error;

    // 4) Validate expected items in the parsed structure
    {
        cJSON *pi = cJSON_GetObjectItemCaseSensitive(parsed, "pi");
        if (!pi || !cJSON_IsNumber(pi)) goto error;
        if (!(pi->valuedouble > 3.14 && pi->valuedouble < 3.15)) goto error;

        cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (!name || !cJSON_IsString(name) || name->valuestring == NULL) goto error;

        cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
        if (!items || !cJSON_IsArray(items) || cJSON_GetArraySize(items) != 3) goto error;
    }

    // 5) Replace one object member with a new value
    cJSON_ReplaceItemInObject(parsed, "name", cJSON_CreateString("modified"));
    {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
        if (!name || !cJSON_IsString(name) || name->valuestring == NULL) goto error;
        // quick check: ensure the new string differs from the old one
        if (strcmp(name->valuestring, "modified") != 0) goto error;
    }

    // 6) Detach an item from the items array and re-attach it (demonstrates detach & add)
    {
        cJSON *items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
        if (!items || !cJSON_IsArray(items)) goto error;

        // detach the second element (index 1)
        detached = cJSON_DetachItemFromArray(items, 1);
        if (!detached) goto error;

        // re-add it to the end of the array
        cJSON_AddItemToArray(items, detached);
        // ownership transferred back to items, so clear detached pointer to avoid double-delete
        detached = NULL;
    }

    // 7) Remove an object member (delete it from parsed)
    cJSON_DeleteItemFromObject(parsed, "failed"); // safe even if present or not

    // 8) Serialize the modified parsed structure to a pretty string (as final check)
    printed2 = cJSON_Print(parsed);
    if (!printed2) goto error;

    // Clean up and return success
    free(printed2);
    printed2 = NULL;
    cJSON_Delete(parsed);
    parsed = NULL;

    return 66;

error:
    if (printed) free(printed);
    if (printed2) free(printed2);
    if (detached) cJSON_Delete(detached);
    if (root) cJSON_Delete(root);
    if (parsed) cJSON_Delete(parsed);
    // arr is deleted when root is deleted; if root deleted earlier arr ownership also handled.
    return -1;
}