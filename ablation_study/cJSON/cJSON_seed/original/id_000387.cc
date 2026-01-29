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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 3.14159);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *retrieved_num = cJSON_GetObjectItemCaseSensitive(root, "value");
    double retrieved_value = cJSON_GetNumberValue(retrieved_num);
    cJSON *retrieved_bool = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool retrieved_flag = cJSON_IsTrue(retrieved_bool);

    // step 3: Operate and Validate
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "double_value", retrieved_value * 2.0);
    cJSON_AddNumberToObject(summary, "truncated_int", (double)(int)retrieved_value);
    cJSON_AddBoolToObject(summary, "flag_copy", retrieved_flag);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}