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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls:
    // creation, adding items, arrays, duplication, printing, minifying,
    // parsing, accessing, replacing, detaching, deleting, and cleanup.
    // On success the function returns 66.

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return -1;
    }

    // Add a few simple members
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddNumberToObject(root, "value", 42);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nullable", cJSON_CreateNull());

    // Create a nested object "meta" with an array "tags"
    cJSON *meta = cJSON_CreateObject();
    if (!meta) {
        cJSON_Delete(root);
        return -2;
    }
    cJSON *tags = cJSON_CreateArray();
    if (!tags) {
        cJSON_Delete(meta);
        cJSON_Delete(root);
        return -3;
    }
    cJSON_AddItemToArray(tags, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(meta, "tags", tags);
    cJSON_AddItemToObject(root, "meta", meta);

    // Create an "items" array with one object inside
    cJSON *items = cJSON_CreateArray();
    if (!items) {
        cJSON_Delete(root);
        return -4;
    }
    cJSON *item1 = cJSON_CreateObject();
    if (!item1) {
        cJSON_Delete(root);
        return -5;
    }
    cJSON_AddStringToObject(item1, "id", "item1");
    cJSON_AddNumberToObject(item1, "qty", 10);
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToObject(root, "items", items);

    // Print the JSON to a formatted string
    char *printed = cJSON_Print(root); // caller must free() this
    if (!printed) {
        cJSON_Delete(root);
        return -6;
    }

    // Deep duplicate the root
    cJSON *dup = cJSON_Duplicate(root, 1); // 1 => recursively duplicate
    if (!dup) {
        free(printed);
        cJSON_Delete(root);
        return -7;
    }

    // Make a mutable copy of the printed string and minify it in-place
    size_t printed_len = strlen(printed);
    char *minified = (char*)malloc(printed_len + 1);
    if (!minified) {
        free(printed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -8;
    }
    memcpy(minified, printed, printed_len + 1);
    cJSON_Minify(minified); // removes whitespace

    // Parse the minified JSON back into a cJSON structure
    cJSON *parsed = cJSON_Parse(minified);
    free(minified);
    if (!parsed) {
        free(printed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -9;
    }

    // Access some members from the parsed object
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!(name && cJSON_IsString(name) && (name->valuestring != NULL))) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(dup);
        cJSON_Delete(root);
        return -10;
    }

    // Replace the "value" member in parsed with a new number
    cJSON_ReplaceItemInObject(parsed, "value", cJSON_CreateNumber(100));

    // Detach the "items" array from parsed and re-add it under a different key
    cJSON *detached_items = cJSON_DetachItemFromObject(parsed, "items");
    if (detached_items) {
        cJSON_AddItemToObject(parsed, "items_copy", detached_items);
    }

    // If meta->tags is an array, delete the first tag element (index 0)
    cJSON *parsed_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    if (parsed_meta) {
        cJSON *parsed_tags = cJSON_GetObjectItemCaseSensitive(parsed_meta, "tags");
        if (parsed_tags && cJSON_IsArray(parsed_tags) && cJSON_GetArraySize(parsed_tags) > 0) {
            cJSON_DeleteItemFromArray(parsed_tags, 0);
        }
    }

    // Cleanup all allocated cJSON structures and printed string
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    free(printed);
    cJSON_Delete(root);

    // If we reach here, the sequence succeeded
    return 66;
}