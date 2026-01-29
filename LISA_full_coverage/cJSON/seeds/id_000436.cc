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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"greeting\":\"hello\",\"numbers\":[7,14]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_bool root_is_invalid = cJSON_IsInvalid(root);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    const char ref_str[] = "reference_string";
    cJSON *ref = cJSON_CreateStringReference(ref_str);
    cJSON_AddItemToArray(arr, ref);
    cJSON_AddItemToObject(root, "refarr", arr);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    int numbers_size = cJSON_GetArraySize(numbers);
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_num);
    cJSON *scaled = cJSON_CreateNumber(first_val * 3.5);

    // step 3: Operate
    cJSON_AddItemToArray(arr, scaled);
    int new_arr_size = cJSON_GetArraySize(arr);
    cJSON *at0 = cJSON_GetArrayItem(arr, 0);
    const char *ref_value = cJSON_GetStringValue(at0);
    cJSON_bool at0_invalid = cJSON_IsInvalid(at0);
    double metric = first_val + (double)numbers_size + (double)new_arr_size;
    cJSON *metric_num = cJSON_CreateNumber(metric);
    cJSON_AddItemToObject(root, "metric", metric_num);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Validate & Cleanup
    (void)ref_value;
    (void)root_is_object;
    (void)root_is_invalid;
    (void)at0_invalid;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}