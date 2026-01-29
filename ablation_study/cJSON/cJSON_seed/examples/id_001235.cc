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
//<ID> 1235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double dvals[3] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *double_arr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(root, "doubles", double_arr);
    cJSON *extras = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON *n2 = cJSON_CreateNumber(8.0);
    cJSON *n3 = cJSON_CreateNumber(9.0);
    cJSON_AddItemToArray(extras, n1);
    cJSON_AddItemToArray(extras, n2);
    cJSON_AddItemToArray(extras, n3);
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    // step 2: Configure
    int count = cJSON_GetArraySize(double_arr);
    cJSON *first = cJSON_GetArrayItem(double_arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool is_null = cJSON_IsNull(maybe_null);

    // step 3: Operate & Validate
    cJSON *a0 = cJSON_GetArrayItem(double_arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(double_arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(double_arr, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)count;
    (void)first_val;
    (void)is_null;
    (void)sum;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}