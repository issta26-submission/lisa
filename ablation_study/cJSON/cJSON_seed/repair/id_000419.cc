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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"values\": [1, 2, 3], \"meta\": {\"scale\": 2.5}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *parsed_values = cJSON_GetObjectItem(root, "values");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n10 = cJSON_CreateNumber(10.0);
    cJSON *n20 = cJSON_CreateNumber(20.0);
    cJSON *n30 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(new_arr, n10);
    cJSON_AddItemToArray(new_arr, n20);
    cJSON_AddItemToArray(new_arr, n30);
    cJSON_AddItemToObject(root, "extra", new_arr);

    // step 3: Operate
    cJSON *pv0 = cJSON_GetArrayItem(parsed_values, 0);
    cJSON *pv1 = cJSON_GetArrayItem(parsed_values, 1);
    cJSON *pv2 = cJSON_GetArrayItem(parsed_values, 2);
    double sum_parsed = cJSON_GetNumberValue(pv0) + cJSON_GetNumberValue(pv1) + cJSON_GetNumberValue(pv2);
    cJSON *na0 = cJSON_GetArrayItem(new_arr, 0);
    cJSON *na1 = cJSON_GetArrayItem(new_arr, 1);
    cJSON *na2 = cJSON_GetArrayItem(new_arr, 2);
    double sum_new = cJSON_GetNumberValue(na0) + cJSON_GetNumberValue(na1) + cJSON_GetNumberValue(na2);
    double total = (sum_parsed + sum_new) * scale;
    cJSON *result = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(result, total);
    cJSON_AddItemToObject(root, "combined_scaled_sum", result);

    // step 4: Validate and Cleanup
    double retrieved = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "combined_scaled_sum"));
    (void)retrieved;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}