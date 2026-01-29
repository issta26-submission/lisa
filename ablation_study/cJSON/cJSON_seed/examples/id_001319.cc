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
//<ID> 1319
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s_ref = cJSON_CreateStringReference("gamma_ref");
    cJSON *b = cJSON_CreateBool(1);
    cJSON *flag = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, b);
    cJSON_AddItemToObject(root, "my_array", arr);
    cJSON_AddItemToObject(root, "ref", s_ref);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    cJSON *bool_as_num = cJSON_CreateNumber((double)second_is_bool);
    cJSON_AddItemToObject(root, "bool_as_num", bool_as_num);
    cJSON_DeleteItemFromObject(root, "ref");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}