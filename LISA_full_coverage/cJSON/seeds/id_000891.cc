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
//<ID> 891
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
    float float_vals[3] = {1.5f, 2.5f, 3.5f};
    double double_vals[2] = {10.0, 20.0};
    cJSON *farr = cJSON_CreateFloatArray(float_vals, 3);
    cJSON *darr = cJSON_CreateDoubleArray(double_vals, 2);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON_AddItemToObject(root, "doubles", darr);

    // step 2: Configure
    int count_f = cJSON_GetArraySize(farr);
    int count_d = cJSON_GetArraySize(darr);
    cJSON_AddNumberToObject(meta, "float_count", (double)count_f);
    cJSON_AddNumberToObject(meta, "double_count", (double)count_d);
    cJSON *first_d_item = cJSON_GetArrayItem(darr, 0);
    double first_d = cJSON_GetNumberValue(first_d_item);
    cJSON_AddNumberToObject(meta, "first_double", first_d);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    double total_metric = (double)count_f + (double)count_d + first_d;
    cJSON_AddNumberToObject(meta, "total_metric", total_metric);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "floats");
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}