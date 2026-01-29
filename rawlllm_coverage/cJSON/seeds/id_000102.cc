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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    /* Create a root object */
    cJSON *root = cJSON_CreateObject();
    if (!root) return 1;

    /* Add basic items */
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "value", 42);

    /* Create an array and add items */
    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); return 1; }
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2));

    /* Create nested object */
    cJSON *meta = cJSON_CreateObject();
    if (!meta) { cJSON_Delete(root); return 1; }
    cJSON_AddStringToObject(meta, "type", "demo");
    cJSON_AddItemToObject(root, "meta", meta);

    /* Print to string */
    char *printed = cJSON_Print(root); /* formatted print; safe fallback */
    if (!printed) { cJSON_Delete(root); return 1; }

    /* Parse back the printed string */
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        free(printed);
        cJSON_Delete(root);
        return 1;
    }

    /* Validate parsed content */
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL) || strcmp(name->valuestring, "example") != 0) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }

    cJSON *value = cJSON_GetObjectItemCaseSensitive(parsed, "value");
    if (!cJSON_IsNumber(value) || value->valuedouble != 42) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }

    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!cJSON_IsArray(parsed_items) || cJSON_GetArraySize(parsed_items) != 2) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }

    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    if (!cJSON_IsString(first_item) || strcmp(first_item->valuestring, "first") != 0) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }

    cJSON *parsed_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    if (!cJSON_IsObject(parsed_meta)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }
    cJSON *meta_type = cJSON_GetObjectItemCaseSensitive(parsed_meta, "type");
    if (!cJSON_IsString(meta_type) || strcmp(meta_type->valuestring, "demo") != 0) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }

    /* Duplicate the original tree (deep copy) and then delete the duplicate */
    cJSON *duplicate = cJSON_Duplicate(root, 1); /* recurse = 1 */
    if (!duplicate) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }
    cJSON_Delete(duplicate);

    /* Detach the second item from the original items array and verify it's the number 2 */
    cJSON *detached = cJSON_DetachItemFromArray(items, 1); /* index 1 */
    if (!detached || !cJSON_IsNumber(detached) || detached->valuedouble != 2) {
        if (detached) cJSON_Delete(detached);
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return 1;
    }
    /* delete the detached item since it's no longer in the array/tree */
    cJSON_Delete(detached);

    /* Cleanup */
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(root);

    /* Success: return 66 as requested */
    return 66;
}