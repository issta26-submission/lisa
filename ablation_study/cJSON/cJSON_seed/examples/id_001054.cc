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
//<ID> 1054
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", f);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *vals_orig = cJSON_GetObjectItem(root, "values");
    int count_orig = cJSON_GetArraySize(vals_orig);
    cJSON *count_orig_num = cJSON_CreateNumber((double)count_orig);
    cJSON_AddItemToObject(dup, "original_count", count_orig_num);

    // step 3: Operate & Validate
    cJSON *vals_dup = cJSON_GetObjectItem(dup, "values");
    int count_dup = cJSON_GetArraySize(vals_dup);
    cJSON *counts_sum = cJSON_CreateNumber((double)(count_orig + count_dup));
    cJSON_AddItemToObject(dup, "counts_sum", counts_sum);
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}