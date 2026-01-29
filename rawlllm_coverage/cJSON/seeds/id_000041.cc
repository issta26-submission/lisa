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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls:
    // create objects/arrays/primitives, add items, print to string,
    // parse back, validate contents, and clean up.
    //
    // On success the function returns 66.

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Add a string, number and boolean to root
    cJSON *name = cJSON_CreateString("example");
    if (!name) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "name", name);

    // convenience helpers
    if (!cJSON_AddNumberToObject(root, "value", 42)) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddBoolToObject(root, "active", 1)) { cJSON_Delete(root); return 0; }

    // Create an array and add some items
    cJSON *items = cJSON_CreateArray();
    if (!items) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("a"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2));

    // Create nested object with an array
    cJSON *meta = cJSON_CreateObject();
    if (!meta) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(root, "meta", meta);
    if (!cJSON_AddStringToObject(meta, "id", "meta1")) { cJSON_Delete(root); return 0; }
    cJSON *flags = cJSON_CreateArray();
    if (!flags) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToObject(meta, "flags", flags);
    cJSON_AddItemToArray(flags, cJSON_CreateBool(0));
    cJSON_AddItemToArray(flags, cJSON_CreateBool(1));

    // Print JSON to string
    char *printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return 0; }

    // Parse back the printed JSON
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) { free(printed); cJSON_Delete(root); return 0; }

    // Validate parsed contents
    cJSON *p_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(p_name) || !p_name->valuestring || std::strcmp(p_name->valuestring, "example") != 0) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }

    cJSON *p_value = cJSON_GetObjectItemCaseSensitive(parsed, "value");
    if (!cJSON_IsNumber(p_value) || (int)(p_value->valuedouble) != 42) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }

    cJSON *p_active = cJSON_GetObjectItemCaseSensitive(parsed, "active");
    if (!cJSON_IsBool(p_active) || p_active->valueint != 1) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }

    cJSON *p_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!cJSON_IsArray(p_items) || cJSON_GetArraySize(p_items) != 3) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }
    // Check elements of the array
    cJSON *it0 = cJSON_GetArrayItem(p_items, 0);
    cJSON *it1 = cJSON_GetArrayItem(p_items, 1);
    cJSON *it2 = cJSON_GetArrayItem(p_items, 2);
    if (!cJSON_IsString(it0) || !it0->valuestring || std::strcmp(it0->valuestring, "a") != 0) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }
    if (!cJSON_IsNumber(it1) || (int)(it1->valuedouble) != 1) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }
    if (!cJSON_IsNumber(it2) || (int)(it2->valuedouble) != 2) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }

    // Check nested meta.flags
    cJSON *p_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    if (!cJSON_IsObject(p_meta)) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }
    cJSON *p_flags = cJSON_GetObjectItemCaseSensitive(p_meta, "flags");
    if (!cJSON_IsArray(p_flags) || cJSON_GetArraySize(p_flags) != 2) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }
    cJSON *flag0 = cJSON_GetArrayItem(p_flags, 0);
    cJSON *flag1 = cJSON_GetArrayItem(p_flags, 1);
    if (!cJSON_IsBool(flag0) || !cJSON_IsBool(flag1)) {
        cJSON_Delete(parsed); free(printed); cJSON_Delete(root); return 0;
    }

    // Clean up
    cJSON_Delete(parsed);
    free(printed);
    cJSON_Delete(root);

    // Success
    return 66;
}