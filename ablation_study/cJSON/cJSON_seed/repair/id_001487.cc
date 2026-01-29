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
//<ID> 1487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[10,20,30],\"meta\":{\"value\":7.7}}";
    size_t len = (size_t)strlen(json);
    const char *endptr = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &endptr, 1);

    // step 2: Configure
    cJSON *nums = cJSON_GetObjectItemCaseSensitive(root, "nums");
    cJSON *i0 = cJSON_GetArrayItem(nums, 0);
    cJSON *i1 = cJSON_GetArrayItem(nums, 1);
    cJSON *i2 = cJSON_GetArrayItem(nums, 2);
    cJSON *copy = cJSON_CreateArray();
    cJSON *d0 = cJSON_Duplicate(i0, 1);
    cJSON *d1 = cJSON_Duplicate(i1, 1);
    cJSON *d2 = cJSON_Duplicate(i2, 1);
    cJSON_AddItemToArray(copy, d0);
    cJSON_AddItemToArray(copy, d1);
    cJSON_AddItemToArray(copy, d2);
    cJSON *result = cJSON_CreateObject();
    cJSON_AddItemToObject(result, "copied", copy);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *val_item = cJSON_GetObjectItemCaseSensitive(meta, "value");
    double meta_val = cJSON_GetNumberValue(val_item);
    cJSON *meta_num = cJSON_CreateNumber(meta_val);
    cJSON_AddItemToObject(result, "meta_value", meta_num);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(copy, 1);
    cJSON *a0 = cJSON_GetArrayItem(copy, 0);
    cJSON *a1 = cJSON_GetArrayItem(copy, 1);
    double n0 = cJSON_GetNumberValue(a0);
    double n1 = cJSON_GetNumberValue(a1);
    double combined = n0 + n1 + meta_val;
    cJSON *combined_item = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(result, "combined", combined_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(result);
    // API sequence test completed successfully
    return 66;
}