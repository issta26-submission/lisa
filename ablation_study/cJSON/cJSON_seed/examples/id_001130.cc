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
//<ID> 1130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *meta_ref = cJSON_CreateStringReference("generated");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObjectCS(root, "enabled", flag);
    cJSON_AddItemToObjectCS(root, "meta", meta_ref);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON_bool t = cJSON_IsTrue(flag);
    double sum = v0 + v1 + (double)t;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}