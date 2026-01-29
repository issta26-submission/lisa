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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *nameStr = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", nameStr);
    cJSON *threshold = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", nullable);
    double *values = (double *)cJSON_malloc(sizeof(double) * 3);
    values[0] = 1.0;
    values[1] = 2.5;
    values[2] = 3.75;
    cJSON *valuesArr = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(config, "values", valuesArr);
    cJSON_free(values);

    // step 2: Operate
    cJSON *fetched_threshold = cJSON_GetObjectItem(config, "threshold");
    double threshold_value = cJSON_GetNumberValue(fetched_threshold);
    cJSON *first_value_item = cJSON_GetArrayItem(valuesArr, 0);
    double first_value = cJSON_GetNumberValue(first_value_item);
    double combined = threshold_value + first_value;

    // step 3: Validate
    (void)combined;
    cJSON *fetched_null = cJSON_GetObjectItem(config, "optional");
    cJSON_bool isNull = cJSON_IsNull(fetched_null);
    (void)isNull;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}