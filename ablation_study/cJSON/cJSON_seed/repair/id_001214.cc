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
//<ID> 1214
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *scale = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(meta, "scale", scale);

    // step 2: Configure
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);
    cJSON *extra = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_AddItemToArray(values, detached_meta);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double base_sum = first_val + second_val;
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    unsigned char first_byte = (unsigned char)out[0];
    double extra_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "extra"));
    double final_value = base_sum + extra_val + (double)first_byte;
    cJSON *result = cJSON_CreateNumber(final_value);
    cJSON_AddItemToObject(root, "result", result);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}