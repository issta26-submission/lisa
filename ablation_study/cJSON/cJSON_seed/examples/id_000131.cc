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
//<ID> 131
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

    // step 2: Configure
    double samples[4] = {0.5, 1.5, 2.5, 3.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(samples, 4);
    cJSON_AddItemToObject(metrics, "samples", double_arr);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "list", list);
    cJSON *n1 = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(list, n1);
    cJSON *n2 = cJSON_CreateNumber(200.0);
    cJSON_AddItemToArray(list, n2);
    cJSON *mean = cJSON_AddNumberToObject(metrics, "mean", 2.5);

    // step 3: Operate & Validate
    int sample_count = cJSON_GetArraySize(double_arr);
    cJSON *second_sample = cJSON_GetArrayItem(double_arr, 1);
    double second_value = cJSON_GetNumberValue(second_sample);
    cJSON_bool first_is_number = cJSON_IsNumber(cJSON_GetArrayItem(double_arr, 0));
    cJSON *flag = cJSON_CreateBool(first_is_number);
    cJSON_AddItemToObject(root, "first_was_number", flag);
    cJSON_AddNumberToObject(root, "sample_count", (double)sample_count);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}