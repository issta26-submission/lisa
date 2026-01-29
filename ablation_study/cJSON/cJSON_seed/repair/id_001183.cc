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
//<ID> 1183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", array);
    double doubles[] = {1.5, 2.5, 3.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(doubles, 3);
    cJSON_AddItemToObject(root, "double_array", double_arr);
    float floats[] = {4.5f, 5.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(floats, 2);
    cJSON_AddItemToObject(root, "float_array", float_arr);
    cJSON_AddNumberToObject(root, "single", 42.0);
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, cJSON_CreateNumber(8.0));

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *single = cJSON_GetObjectItem(root, "single");
    double single_val = cJSON_GetNumberValue(single);
    double total = first_val + single_val + (double)count;
    cJSON_AddNumberToObject(root, "computed_sum", total);
    cJSON *dup_double_arr = cJSON_Duplicate(double_arr, 1);
    cJSON_AddItemToArray(array, dup_double_arr);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}