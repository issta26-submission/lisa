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
//<ID> 871
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
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 2.0);
    cJSON *first = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, first);
    cJSON *second = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, second);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *duplicated = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToArray(arr, duplicated);
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double set_result = cJSON_SetNumberHelper(count_item, 5.0);
    (void)set_result;

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(arr, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double sum = v0 + v1 + v2 + cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "sum", sum);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}