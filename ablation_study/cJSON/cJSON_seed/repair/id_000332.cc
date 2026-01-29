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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToArray(nums, n3);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON *status = cJSON_CreateString("old_status");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *new_status = cJSON_CreateString("new_status");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "status", new_status);

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(nums, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "selected_value", second_val);

    // step 4: Validate and Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}