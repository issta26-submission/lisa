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
//<ID> 48
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
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "values", arr);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 0.0);
    cJSON *enabled_flag = cJSON_CreateTrue();
    cJSON_bool added_enabled = cJSON_AddItemToObject(root, "enabled", enabled_flag);
    cJSON *visible_flag = cJSON_CreateFalse();
    cJSON_bool added_visible = cJSON_AddItemToObject(root, "visible", visible_flag);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON_bool ins0 = cJSON_InsertItemInArray(arr, 0, n1);
    cJSON_bool ins1 = cJSON_InsertItemInArray(arr, 1, n2);
    int arr_size = cJSON_GetArraySize(arr);
    double prev_count = cJSON_SetNumberHelper(count_item, (double)arr_size);
    (void)prev_count;

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double total = v0 + v1;
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", total);
    cJSON *read_sum = cJSON_GetObjectItem(root, "sum");
    double sum_read_back = cJSON_GetNumberValue(read_sum);
    cJSON *enabled_read = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_read);
    cJSON *visible_read = cJSON_GetObjectItem(root, "visible");
    cJSON_bool is_visible = cJSON_IsFalse(visible_read);
    int summary = (int)arr_size + (int)is_enabled + (int)is_visible + (int)(sum_read_back);
    (void)added_arr;
    (void)added_enabled;
    (void)added_visible;
    (void)ins0;
    (void)ins1;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}