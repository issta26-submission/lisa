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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);

    // step 2: Configure
    double nums[] = {1.1, 2.2, 3.3, 4.4};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(metrics, "double_series", dbl_arr);
    cJSON *cnt = cJSON_AddNumberToObject(metrics, "count", 4.0);
    cJSON *extra_num = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(values, extra_num);

    // step 3: Operate & Validate
    cJSON *retrieved_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *retrieved_dbl_arr = cJSON_GetObjectItem(retrieved_metrics, "double_series");
    int dbl_count = cJSON_GetArraySize(retrieved_dbl_arr);
    cJSON *first_item = cJSON_GetArrayItem(retrieved_dbl_arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *first_added = cJSON_AddNumberToObject(root, "first_value", first_val);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}