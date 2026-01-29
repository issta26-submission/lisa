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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add basic types
    if (!cJSON_AddStringToObject(root, "name", "cJSON Test")) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddNumberToObject(root, "version", 1)) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(root, "enabled", cJSON_CreateBool(1))) { cJSON_Delete(root); return 0; }
    if (!cJSON_AddItemToObject(root, "optional", cJSON_CreateNull())) { cJSON_Delete(root); return 0; }

    // Create an array "values" and add numbers
    cJSON *values = cJSON_CreateArray();
    if (!values) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30));
    cJSON_AddItemToObject(root, "values", values);

    // Create nested object "meta" with an array of tags
    cJSON *meta = cJSON_CreateObject();
    if (!meta) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(meta, "author", "Example");
    cJSON *tags = cJSON_CreateArray();
    if (!tags) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(tags, cJSON_CreateString("json"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("test"));
    cJSON_AddItemToObject(meta, "tags", tags);
    cJSON_AddItemToObject(root, "meta", meta);

    // Print the JSON (unformatted)
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return 0; }

    // Parse back the printed string
    cJSON *parsed = cJSON_Parse(printed);
    // Free the printed string (cJSON allocates the string with the same allocator; use cJSON_free if available)
#ifdef cJSON_free
    cJSON_free(printed);
#else
    free(printed);
#endif
    if (!parsed) { cJSON_Delete(root); return 0; }

    // Validate parsed content
    cJSON *it_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(it_name) || (it_name->valuestring == NULL)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    cJSON *it_version = cJSON_GetObjectItemCaseSensitive(parsed, "version");
    if (!cJSON_IsNumber(it_version)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    cJSON *it_enabled = cJSON_GetObjectItemCaseSensitive(parsed, "enabled");
    if (!cJSON_IsBool(it_enabled)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    cJSON *it_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    if (!cJSON_IsArray(it_values)) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    int arr_size = cJSON_GetArraySize(it_values);
    if (arr_size != 3) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    cJSON *first = cJSON_GetArrayItem(it_values, 0);
    if (!cJSON_IsNumber(first) || first->valuedouble != 10) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    // Duplicate the parsed tree (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    if (!dup) { cJSON_Delete(parsed); cJSON_Delete(root); return 0; }

    // Detach the "optional" item from the duplicate (should be a cJSON_NULL)
    cJSON *detached_optional = cJSON_DetachItemFromObject(dup, "optional");
    // If detached_optional is non-null, delete it separately
    if (detached_optional) cJSON_Delete(detached_optional);

    // Replace "version" with a new number in the duplicate
    cJSON_ReplaceItemInObject(dup, "version", cJSON_CreateNumber(2));

    // Detach the second element (index 1) from the "values" array in the duplicate
    cJSON *dup_values = cJSON_GetObjectItemCaseSensitive(dup, "values");
    if (cJSON_IsArray(dup_values)) {
        cJSON *detached_value = cJSON_DetachItemFromArray(dup_values, 1);
        if (detached_value) cJSON_Delete(detached_value);
        // After detaching, array size should be 2
        if (cJSON_GetArraySize(dup_values) != 2) { cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 0; }
    } else {
        cJSON_Delete(dup); cJSON_Delete(parsed); cJSON_Delete(root); return 0;
    }

    // Clean up all allocated cJSON objects
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence of API calls worked as expected
    return 66;
}