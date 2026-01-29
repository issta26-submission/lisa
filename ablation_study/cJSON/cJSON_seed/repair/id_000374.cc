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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    const char *gval = cJSON_GetStringValue(greeting);
    cJSON *gcopy = cJSON_CreateString(gval);
    cJSON_AddItemToObject(root, "greeting_copy", gcopy);
    cJSON_bool is_null = cJSON_IsNull(optional);
    cJSON *null_flag = cJSON_CreateNumber((double)is_null);
    cJSON_AddItemToObject(root, "null_flag", null_flag);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "greeting_copy");
    const char *retrieved_val = cJSON_GetStringValue(retrieved);
    cJSON *echo = cJSON_CreateString(retrieved_val);
    cJSON_AddItemToObject(root, "echo", echo);
    double flag_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "null_flag")) + (double)cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    cJSON *sum_num = cJSON_CreateNumber(flag_sum);
    cJSON_AddItemToObject(root, "flag_sum", sum_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}