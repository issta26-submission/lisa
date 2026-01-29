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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // Root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    // Basic scalar additions
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 3.14);
    cJSON_AddItemToObject(root, "valid", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "missing", cJSON_CreateNull());

    // Create and populate an array, then add it to root
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 2; }
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(arr, cJSON_CreateString("thirty"));
    cJSON_AddItemToObject(root, "items", arr);

    // Print pretty (allocated string)
    char *pretty = cJSON_Print(root);
    if (!pretty) {
        cJSON_Delete(root);
        return 3;
    }

    // Duplicate the whole structure (deep copy)
    cJSON *copy = cJSON_Duplicate(root, /*recurse=*/1);
    if (!copy) {
        free(pretty);
        cJSON_Delete(root);
        return 4;
    }

    // Access object members safely
    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL)) {
        free(pretty);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 5;
    }

    // Work with the array: compute sum of numeric items
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    if (!items || !cJSON_IsArray(items)) {
        free(pretty);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 6;
    }
    int arr_size = cJSON_GetArraySize(items);
    double sum = 0.0;
    for (int i = 0; i < arr_size; ++i) {
        cJSON *it = cJSON_GetArrayItem(items, i);
        if (cJSON_IsNumber(it)) sum += it->valuedouble;
    }

    // Detach the first element of the array and attach it to root under another key
    cJSON *first_elem = cJSON_DetachItemFromArray(items, 0);
    if (first_elem) {
        cJSON_AddItemToObject(root, "first_item", first_elem); // transfer ownership
    }

    // Replace "value" by removing and adding a new number
    cJSON *old_value = cJSON_DetachItemFromObject(root, "value");
    if (old_value) cJSON_Delete(old_value);
    cJSON_AddNumberToObject(root, "value", 2.71828);

    // Print compact/unformatted
    char *compact = cJSON_PrintUnformatted(root);
    if (!compact) {
        free(pretty);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 7;
    }

    // Make a writable copy of the compact JSON and minify it in-place
    size_t compact_len = strlen(compact);
    char *minifiable = (char*)malloc(compact_len + 1);
    if (!minifiable) {
        free(pretty);
        free(compact);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 8;
    }
    strcpy(minifiable, compact);
    cJSON_Minify(minifiable);

    // Parse the minified JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(minifiable);
    if (!parsed) {
        free(pretty);
        free(compact);
        free(minifiable);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 9;
    }

    // Quick validation of parsed content
    cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(pname) || (pname->valuestring == NULL)) {
        free(pretty);
        free(compact);
        free(minifiable);
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 10;
    }

    // Cleanup all allocated cJSON structures and strings
    free(pretty);
    free(compact);
    free(minifiable);
    cJSON_Delete(parsed);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // If we reached here, the API sequence ran successfully
    return 66;
}