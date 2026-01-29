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
//<ID> 133
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
    cJSON *series = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "series", series);

    // step 2: Configure
    double values[3];
    values[0] = 1.5;
    values[1] = 2.5;
    values[2] = 3.5;
    cJSON *dblarr = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToArray(series, dblarr);
    cJSON *count_num = cJSON_AddNumberToObject(metrics, "count", 3.0);

    // step 3: Operate & Validate
    cJSON *retr_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *retr_count = cJSON_GetObjectItem(retr_metrics, "count");
    double count_val = cJSON_GetNumberValue(retr_count);
    cJSON *retr_series = cJSON_GetObjectItem(retr_metrics, "series");
    int arr_size = cJSON_GetArraySize(retr_series);
    cJSON *first_nested = cJSON_GetArrayItem(retr_series, 0);
    cJSON *first_value = cJSON_GetArrayItem(first_nested, 0);
    double first_val = cJSON_GetNumberValue(first_value);
    cJSON *validated = cJSON_CreateBool((cJSON_bool)(first_val == 1.5 && arr_size == 1 && count_val == 3.0));
    cJSON_AddItemToObject(root, "validated", validated);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}