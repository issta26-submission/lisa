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
//<ID> 610
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(nums, n0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemReferenceToArray(nums, n1);
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON_AddNumberToObject(root, "total", 0.0);
    cJSON_AddNullToObject(root, "temp");

    // step 2: Configure
    cJSON *nums_ref = cJSON_GetObjectItem(root, "numbers");
    cJSON *a0 = cJSON_GetArrayItem(nums_ref, 0);
    cJSON *a1 = cJSON_GetArrayItem(nums_ref, 1);
    cJSON *a2 = cJSON_GetArrayItem(nums_ref, 2);
    cJSON *a3 = cJSON_GetArrayItem(nums_ref, 3);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double v3 = cJSON_GetNumberValue(a3);
    double sum = v0 + v1 + v2 + v3;
    cJSON *total = cJSON_GetObjectItem(root, "total");
    cJSON_SetNumberHelper(total, sum);

    // step 3: Operate
    cJSON_DeleteItemFromObject(root, "temp");
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON *detached_nums = cJSON_DetachItemFromObjectCaseSensitive(root, "numbers");
    char *detached_snapshot = cJSON_PrintUnformatted(detached_nums);
    double final_total = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "total"));
    (void)final_total;
    if (snapshot) { cJSON_free(snapshot); }
    if (detached_snapshot) { cJSON_free(detached_snapshot); }
    cJSON_Delete(detached_nums);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}