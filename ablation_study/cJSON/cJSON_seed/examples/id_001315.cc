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
//<ID> 1315
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
    cJSON *s1 = cJSON_CreateString("value1");
    cJSON *sref = cJSON_CreateStringReference("value_ref");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    size_t len = strlen(first_val);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, first_val, len + 1);
    cJSON *enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_bool = cJSON_IsBool(enabled);
    (void)is_bool;
    cJSON_DeleteItemFromObject(root, "enabled");

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}