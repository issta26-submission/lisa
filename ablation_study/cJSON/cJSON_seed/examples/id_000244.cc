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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *values_arr = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_bool added_a = cJSON_AddItemToArray(values_arr, num_a);
    cJSON_bool added_b = cJSON_AddItemToArray(values_arr, num_b);
    double set_b = cJSON_SetNumberHelper(num_b, 2.718281828);
    cJSON_bool added_values = cJSON_AddItemToObject(root_obj, "values", values_arr);

    // step 3: Operate & Validate
    cJSON *sum = cJSON_CreateNumber(num_a->valuedouble + num_b->valuedouble);
    cJSON_AddItemToObject(root_obj, "sum", sum);
    double doubled_sum = cJSON_SetNumberHelper(sum, sum->valuedouble * 2.0);
    cJSON *doubled_sum_num = cJSON_CreateNumber(doubled_sum);
    cJSON_AddItemToObject(root_obj, "sum_doubled", doubled_sum_num);
    cJSON *op_flags = cJSON_CreateArray();
    cJSON_AddItemToArray(op_flags, cJSON_CreateNumber((double)added_a));
    cJSON_AddItemToArray(op_flags, cJSON_CreateNumber((double)added_b));
    cJSON_AddItemToArray(op_flags, cJSON_CreateNumber((double)added_values));
    cJSON_AddItemToObject(root_obj, "op_flags", op_flags);

    // step 4: Cleanup
    cJSON_Delete(root_obj);
    return 66;
    // API sequence test completed successfully
}