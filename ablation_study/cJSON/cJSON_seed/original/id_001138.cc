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
//<ID> 1138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *ref_item = cJSON_CreateStringReference("initial");
    cJSON_AddItemToArray(arr, ref_item);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 2: Configure
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddItemToObject(root, "always_true", cJSON_CreateTrue());
    cJSON *replacement = cJSON_CreateStringReference("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON_AddBoolToObject(root, "replaced_flag", replaced);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "first_value_copy", first_str);
    char *flat = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}