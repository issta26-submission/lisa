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
//<ID> 1050
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
    cJSON *ffalse = cJSON_CreateFalse();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);

    // step 2: Configure
    cJSON_bool added_n1 = cJSON_AddItemToArray(arr, n1);
    cJSON_bool added_n2 = cJSON_AddItemToArray(arr, n2);
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "nums", arr);
    cJSON_bool added_flag = cJSON_AddItemToObject(root, "flag", ffalse);
    cJSON *missing = cJSON_AddNullToObject(root, "missing");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON_bool is_missing_null = cJSON_IsNull(missing);
    cJSON *count_num = cJSON_CreateNumber((double)count + (double)is_missing_null);
    cJSON_bool added_count = cJSON_AddItemToObject(root, "count", count_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}