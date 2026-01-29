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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(5.5);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.0);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(values, 0);
    cJSON *it1 = cJSON_GetArrayItem(values, 1);
    cJSON *it2 = cJSON_GetArrayItem(values, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double v2 = cJSON_GetNumberValue(it2);
    double sum = v0 + v1 + v2;
    cJSON *scale_obj = cJSON_GetObjectItem(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_obj);
    double scaled_sum = sum * scale_val;
    cJSON_AddNumberToObject(root, "scaled_sum", scaled_sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}