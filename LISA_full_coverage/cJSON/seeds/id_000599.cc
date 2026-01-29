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
//<ID> 599
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums_arr[3] = {10.5, 20.25, 30.125};
    cJSON *root = cJSON_CreateObject();
    cJSON *values_array = cJSON_CreateDoubleArray(nums_arr, 3);
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON *avg_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "avg", avg_item);

    // step 2: Configure
    cJSON *values_ref = cJSON_GetObjectItem(root, "values");
    cJSON *item0 = cJSON_GetArrayItem(values_ref, 0);
    cJSON *item1 = cJSON_GetArrayItem(values_ref, 1);
    cJSON *item2 = cJSON_GetArrayItem(values_ref, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double average = (v0 + v1 + v2) / 3.0;
    cJSON_SetNumberHelper(avg_item, average);

    // step 3: Operate
    cJSON *new_avg = cJSON_CreateNumber(average + 1.0);
    cJSON_ReplaceItemViaPointer(root, avg_item, new_avg);

    // step 4: Validate & Cleanup
    cJSON *avg_after = cJSON_GetObjectItem(root, "avg");
    double final_avg = cJSON_GetNumberValue(avg_after);
    (void)final_avg;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}