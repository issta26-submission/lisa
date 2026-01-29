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
//<ID> 134
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
    cJSON_AddNumberToObject(metrics, "mean", 12.34);
    cJSON_AddNumberToObject(metrics, "max", 99.0);
    double samples_arr[] = {1.1, 2.2, 3.3};
    cJSON *samples = cJSON_CreateDoubleArray(samples_arr, 3);
    cJSON_AddItemToObject(metrics, "samples", samples);
    cJSON *v1 = cJSON_CreateNumber(10.0);
    cJSON *v2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}