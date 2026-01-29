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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API usages:
    // - create objects/arrays/values
    // - add/replace/detach items
    // - duplicate objects (deep copy)
    // - print/parse JSON text
    // - query items and array size
    // - clean up
    //
    // On successful completion the function returns 66.
    // On error it returns a non-66 value (small positive int).
    //
    // Note: This function assumes the cJSON header is already included in the
    // translation unit that will compile it and that the environment provides
    // free() and strcmp() (standard C library). Adjust includes in your source
    // file as needed.

    // 1) Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // 2) Add simple members: string and number
    cJSON_AddStringToObject(root, "name", "cJSON test");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // 3) Create an array and add it to the root object
    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToObject(root, "items", items);

    // 4) Populate the array with values
    cJSON_AddItemToArray(items, cJSON_CreateString("one"));
    cJSON_AddItemToArray(items, cJSON_CreateString("two"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3));

    // 5) Duplicate the root (deep duplicate)
    cJSON *dup = cJSON_Duplicate(root, /*recurse*/ 1);
    if (!dup) { cJSON_Delete(root); return 3; }

    // 6) Replace a member in the duplicate to show mutation on the copy
    cJSON_ReplaceItemInObject(dup, "name", cJSON_CreateString("duplicated"));

    // 7) Serialize both original and duplicate to compact JSON strings
    char *str_root = cJSON_PrintUnformatted(root);
    char *str_dup  = cJSON_PrintUnformatted(dup);
    if (!str_root || !str_dup) {
        if (str_root) free(str_root);
        if (str_dup)  free(str_dup);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return 4;
    }

    // 8) Parse back the root string to produce a new cJSON structure
    cJSON *parsed = cJSON_Parse(str_root);
    if (!parsed) {
        free(str_root);
        free(str_dup);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return 5;
    }

    // 9) Validate a parsed member ("name" should equal the original)
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!nameItem || !cJSON_IsString(nameItem) || strcmp(nameItem->valuestring, "cJSON test") != 0) {
        free(str_root);
        free(str_dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return 6;
    }

    // 10) Validate the items array and its size
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!parsed_items || !cJSON_IsArray(parsed_items) || cJSON_GetArraySize(parsed_items) != 3) {
        free(str_root);
        free(str_dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return 7;
    }

    // 11) Detach the second element (index 1) from the duplicate's array and
    //     append it to the original root's array (demonstrates detach + add)
    cJSON *dup_items = cJSON_GetObjectItemCaseSensitive(dup, "items");
    cJSON *detached = NULL;
    if (dup_items) detached = cJSON_DetachItemFromArray(dup_items, 1); // detach "two"
    if (!detached) {
        // If detach failed, still continue to cleanup but report error
        free(str_root);
        free(str_dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        cJSON_Delete(dup);
        return 8;
    }
    // Append the detached item into the original 'items' array
    cJSON_AddItemToArray(items, detached);
    // Note: 'detached' is now owned by 'items' (root). Do not delete it separately.

    // 12) Optionally: remove a member from duplicate
    cJSON_DeleteItemFromObject(dup, "version"); // safe even if member absent

    // 13) Cleanup: free printed strings and delete cJSON structures
    free(str_root);
    free(str_dup);

    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // Success
    return 66;
}