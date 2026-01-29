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
//<ID> 420
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "threshold", 7.25);
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON *v0 = cJSON_CreateNumber(1.5);
    cJSON *v1 = cJSON_CreateNumber(2.5);
    cJSON *v2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(values, v0);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);
    cJSON_AddItemToObject(root, "values", values);

    // step 3: Operate
    double a0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double a1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double a2 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 2));
    double sum = a0 + a1 + a2;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    cJSON_AddBoolToObject(root, "copy_equals_original", equal);

    // step 4: Validate and Cleanup
    double retrieved_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    (void)retrieved_sum;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}