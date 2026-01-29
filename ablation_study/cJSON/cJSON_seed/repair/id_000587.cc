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
//<ID> 587
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
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.5));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.5));
    cJSON_InsertItemInArray(values, 1, cJSON_CreateNumber(2.5));
    cJSON_AddNumberToObject(metrics, "base", 100.0);

    // step 3: Operate and Validate
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON_bool has_metrics = cJSON_HasObjectItem(root, "metrics");
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(got_metrics, "values");
    int count = cJSON_GetArraySize(got_values);
    cJSON *item0 = cJSON_GetArrayItem(got_values, 0);
    cJSON *item1 = cJSON_GetArrayItem(got_values, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double computed_sum = v0 + v1;
    cJSON_AddNumberToObject(root, "computed_sum", computed_sum);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddNumberToObject(root, "has_metrics", (double)has_metrics);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}