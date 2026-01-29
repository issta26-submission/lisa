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
//<ID> 248
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON *sum_num = cJSON_CreateNumber(6.0);

    // step 2: Configure
    cJSON_bool added_n1 = cJSON_AddItemToArray(arr, n1);
    cJSON_bool added_n2 = cJSON_AddItemToArray(arr, n2);
    cJSON_bool added_n3 = cJSON_AddItemToArray(arr, n3);
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_bool added_sum = cJSON_AddItemToObject(root, "sum", sum_num);

    // step 3: Operate & Validate
    double prev_n2 = cJSON_SetNumberHelper(n2, 20.0);
    double updated_sum = sum_num->valuedouble - prev_n2 + 20.0;
    cJSON_SetNumberHelper(sum_num, updated_sum);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", size_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_Minify(out);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}