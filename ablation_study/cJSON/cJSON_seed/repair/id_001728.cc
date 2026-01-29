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
//<ID> 1728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *timeout = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(config, "timeout", timeout);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddNullToObject(config, "optional");

    // step 2: Configure
    double old_timeout = cJSON_SetNumberHelper(timeout, 45.5);
    (void)old_timeout;
    int nums_arr[3] = {1, 2, 3};
    cJSON *nums = cJSON_CreateIntArray(nums_arr, 3);
    cJSON_AddItemToObject(root, "nums", nums);

    // step 3: Operate and Validate
    cJSON *retr_enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(retr_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = enabled_flag ? 'T' : 'F';
    const char *opt_val = cJSON_GetStringValue(cJSON_GetObjectItem(config, "optional"));
    buffer[2] = opt_val ? opt_val[0] : 'N';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}