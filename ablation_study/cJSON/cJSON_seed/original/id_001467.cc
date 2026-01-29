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
//<ID> 1467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"root\":{\"arr\":[1,2],\"name\":\"orig\"},\"ref\":null}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *arr = cJSON_GetObjectItem(root_obj, "arr");
    cJSON *first = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *dup_first = cJSON_Duplicate(first, 1);
    cJSON_AddItemReferenceToArray(arr, dup_first);
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root_obj, "name", new_name);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *replacement = cJSON_CreateNumber(123.0);
    cJSON_ReplaceItemViaPointer(arr, second, replacement);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    cJSON *ref_node = cJSON_GetObjectItem(root, "ref");
    cJSON_bool ref_invalid = cJSON_IsInvalid(ref_node);
    (void)first_invalid;
    (void)ref_invalid;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}