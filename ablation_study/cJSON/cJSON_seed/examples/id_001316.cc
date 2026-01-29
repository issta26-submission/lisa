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
//<ID> 1316
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
    cJSON *s = cJSON_CreateString("alpha");
    cJSON *s_ref = cJSON_CreateStringReference("alpha_ref");
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToArray(arr, s_ref);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON_bool is_flag_bool = cJSON_IsBool(flag);
    cJSON *flag_num = cJSON_CreateNumber((double)is_flag_bool);
    cJSON_AddItemToObject(root, "flag_num", flag_num);
    cJSON_DeleteItemFromObject(root, "active");

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}