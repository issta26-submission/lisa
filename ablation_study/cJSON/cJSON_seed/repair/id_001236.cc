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
//<ID> 1236
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *sref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(arr, sref);

    // step 2: Configure
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_false", flag_false);
    cJSON_AddItemToObject(root, "flag_true", flag_true);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_arr);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(flag_false);
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    double v0 = cJSON_GetNumberValue(n1);
    double v1 = cJSON_GetNumberValue(n2);
    double computed = v0 + v1 + (double)is_bool + (double)is_false;
    cJSON *result = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", result);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}