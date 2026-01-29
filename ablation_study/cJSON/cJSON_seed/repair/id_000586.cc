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
//<ID> 586
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
    cJSON_AddNumberToObject(metrics, "count", 0.0);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(10.5);
    cJSON_InsertItemInArray(values, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(20.25);
    cJSON_InsertItemInArray(values, 1, n1);
    cJSON_AddNumberToObject(metrics, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *got_values_cs = cJSON_GetObjectItemCaseSensitive(metrics, "values");
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON_bool has_values = cJSON_HasObjectItem(metrics, "values");
    cJSON *first = cJSON_GetArrayItem(got_values_cs, 0);
    double v0 = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", v0);
    cJSON *count_item = cJSON_GetObjectItem(got_metrics, "count");
    double count = cJSON_GetNumberValue(count_item);
    double derived = count + v0;
    cJSON_AddNumberToObject(root, "derived", derived);
    cJSON *n2 = cJSON_CreateNumber(v0 + 1.0);
    cJSON_InsertItemInArray(values, 2, n2);
    cJSON_AddNumberToObject(root, "has_values_flag", (double)has_values);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}