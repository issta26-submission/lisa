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
//<ID> 242
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

    // step 2: Configure
    cJSON_bool added_n1 = cJSON_AddItemToArray(arr, n1);
    cJSON_bool added_n2 = cJSON_AddItemToArray(arr, n2);
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *flag_added_arr = cJSON_CreateNumber((double)added_arr);
    cJSON_AddItemToObject(root, "added_numbers_array", flag_added_arr);

    // step 3: Operate & Validate
    double previous_n2 = cJSON_SetNumberHelper(n2, 3.14159);
    cJSON *prev_n2_num = cJSON_CreateNumber(previous_n2);
    cJSON_AddItemToObject(root, "previous_n2", prev_n2_num);
    double sum_vals = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2);
    cJSON *sum_num = cJSON_CreateNumber(sum_vals);
    cJSON_AddItemToObject(root, "sum", sum_num);
    cJSON *count_num = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "count", count_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}