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
//<ID> 1188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double dnums[3] = {1.1, 2.2, 3.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(dnums, 3);
    float fnums[2] = {4.4f, 5.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(fnums, 2);
    cJSON_AddItemToObject(root, "doubles", double_arr);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON *mixed = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(mixed, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(mixed, s2);
    cJSON_AddItemToObject(root, "mixed", mixed);

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    double a0 = cJSON_GetNumberValue(cJSON_GetArrayItem(double_arr, 0));
    double a1 = cJSON_GetNumberValue(cJSON_GetArrayItem(double_arr, 1));
    double a2 = cJSON_GetNumberValue(cJSON_GetArrayItem(double_arr, 2));
    double f0 = cJSON_GetNumberValue(cJSON_GetArrayItem(float_arr, 0));
    double f1 = cJSON_GetNumberValue(cJSON_GetArrayItem(float_arr, 1));
    double sum = a0 + a1 + a2 + f0 + f1;
    cJSON_AddNumberToObject(root, "sum", sum);
    double retrieved_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    double mean = retrieved_sum / 5.0;
    cJSON_AddNumberToObject(root, "mean", mean);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}