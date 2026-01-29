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
//<ID> 1182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arrays = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arrays", arrays);

    // step 2: Configure
    double dvals[3] = {1.5, 2.5, 3.5};
    float fvals[2] = {4.5f, 5.5f};
    cJSON *d_array = cJSON_CreateDoubleArray(dvals, 3);
    cJSON *f_array = cJSON_CreateFloatArray(fvals, 2);
    cJSON_AddItemToArray(arrays, d_array);
    cJSON_AddItemToArray(arrays, f_array);
    cJSON_AddNumberToObject(root, "count_d", 3.0);
    cJSON_AddNumberToObject(root, "count_f", 2.0);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    double d0 = cJSON_GetNumberValue(cJSON_GetArrayItem(d_array, 0));
    double d1 = cJSON_GetNumberValue(cJSON_GetArrayItem(d_array, 1));
    double d2 = cJSON_GetNumberValue(cJSON_GetArrayItem(d_array, 2));
    double f0 = cJSON_GetNumberValue(cJSON_GetArrayItem(f_array, 0));
    double f1 = cJSON_GetNumberValue(cJSON_GetArrayItem(f_array, 1));
    double total_sum = d0 + d1 + d2 + f0 + f1;
    cJSON_AddNumberToObject(root, "sum", total_sum);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}