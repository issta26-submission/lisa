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
//<ID> 1134
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
    cJSON *first_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, first_true);
    cJSON *str_ref = cJSON_CreateStringReference("referenced_string");
    cJSON_AddItemToArray(arr, str_ref);
    cJSON *null_entry = cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, replacement);
    (void)replaced;

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    char *second_val = cJSON_GetStringValue(second_item);
    cJSON_AddItemToObject(root, "ref_echo", cJSON_CreateStringReference(second_val ? second_val : ""));

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 0);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}