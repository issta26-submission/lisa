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
//<ID> 585
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
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON_bool has_metrics = cJSON_HasObjectItem(root, "metrics");
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *got_values_cs = cJSON_GetObjectItemCaseSensitive(got_metrics, "values");
    cJSON_InsertItemInArray(got_values_cs, 1, cJSON_CreateNumber(4.5));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(got_values_cs, 0);
    cJSON *second = cJSON_GetArrayItem(got_values_cs, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double sum = v0 + v1 + (double)has_metrics;
    cJSON_AddNumberToObject(root, "sum01", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}