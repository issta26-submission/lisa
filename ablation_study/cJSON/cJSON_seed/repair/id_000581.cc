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
//<ID> 581
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
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));
    cJSON_AddNumberToObject(root, "sum_guess", 0.0);

    // step 2: Configure
    cJSON_InsertItemInArray(values, 1, cJSON_CreateNumber(15.0));
    cJSON_AddNumberToObject(root, "count_guess", 4.0);

    // step 3: Operate and Validate
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON_bool has_values = cJSON_HasObjectItem(got_metrics, "values");
    cJSON *got_values_cs = cJSON_GetObjectItemCaseSensitive(got_metrics, "values");
    cJSON *item0 = cJSON_GetArrayItem(got_values_cs, 0);
    cJSON *item1 = cJSON_GetArrayItem(got_values_cs, 1);
    cJSON *item2 = cJSON_GetArrayItem(got_values_cs, 2);
    cJSON *item3 = cJSON_GetArrayItem(got_values_cs, 3);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double v3 = cJSON_GetNumberValue(item3);
    double sum = v0 + v1 + v2 + v3 + (double)has_values * 0.0;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}