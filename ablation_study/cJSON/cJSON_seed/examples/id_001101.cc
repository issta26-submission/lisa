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
//<ID> 1101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(3.14);
    cJSON *n1 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate & Validate
    cJSON *values_ref = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(values_ref);
    int count = cJSON_GetArraySize(values_ref);
    cJSON *count_added = cJSON_AddNumberToObject(root, "count", (double)count);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values_ref, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values_ref, 1));
    double average = (v0 + v1) / 2.0;
    cJSON *avg = cJSON_CreateNumber(average);
    cJSON_AddItemToObject(root, "average", avg);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)values_is_array;
    (void)count_added;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}