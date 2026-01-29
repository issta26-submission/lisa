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
//<ID> 810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 10.0);
    cJSON *settings = cJSON_AddObjectToObject(config, "settings");
    cJSON_AddNumberToObject(settings, "multiplier", 2.5);
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 3: Operate and Validate
    cJSON *cfg_ref = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(cfg_ref, "threshold");
    double threshold = cJSON_GetNumberValue(threshold_item);
    int arr_size = cJSON_GetArraySize(values);
    cJSON *arr_item0 = cJSON_GetArrayItem(values, 0);
    cJSON *arr_item1 = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(arr_item0);
    double v1 = cJSON_GetNumberValue(arr_item1);
    cJSON *detached_settings = cJSON_DetachItemFromObject(config, "settings");

    // step 4: Cleanup
    cJSON_Delete(detached_settings);
    cJSON_Delete(root);
    (void)threshold;
    (void)arr_size;
    (void)v0;
    (void)v1;
    // API sequence test completed successfully
    return 66;
}