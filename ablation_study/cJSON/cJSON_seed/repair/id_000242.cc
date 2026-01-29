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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *samples = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON *n3 = cJSON_CreateNumber(30.75);
    cJSON_AddItemToArray(samples, n1);
    cJSON_AddItemToArray(samples, n2);
    cJSON_AddItemToArray(samples, n3);
    cJSON_AddItemToObject(root, "samples", samples);
    cJSON_AddNumberToObject(root, "threshold", 25.0);
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(samples, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 2));
    double average = (v0 + v1 + v2) / 3.0;
    cJSON_AddNumberToObject(metrics, "avg", average);
    cJSON_AddNumberToObject(metrics, "mid_value", second_value);

    // step 3: Operate and Validate
    cJSON_bool has_samples = cJSON_HasObjectItem(root, "samples");
    cJSON_AddNumberToObject(root, "has_samples", (double)has_samples);
    cJSON_AddNumberToObject(root, "threshold_exceeded_count", (double)(v0 > 25.0) + (double)(v1 > 25.0) + (double)(v2 > 25.0));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}