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
//<ID> 943
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
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(2.0);
    cJSON *bool_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, bool_item);
    cJSON_InsertItemInArray(arr, 1, num_b);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *dup_b = cJSON_Duplicate(num_b, 0);
    cJSON_AddItemToObject(root, "dup_b", dup_b);

    // step 3: Operate & Validate
    cJSON *got_num_a = cJSON_GetArrayItem(arr, 0);
    double v_a = cJSON_GetNumberValue(got_num_a);
    cJSON *got_bool = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_bool = cJSON_IsBool(got_bool);
    cJSON_bool cmp_res = cJSON_Compare(num_b, dup_b, 1);
    double computed_value = v_a + (double)cmp_res + (double)is_bool;
    cJSON *computed_num = cJSON_CreateNumber(computed_value);
    cJSON_AddItemToObject(root, "computed", computed_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}