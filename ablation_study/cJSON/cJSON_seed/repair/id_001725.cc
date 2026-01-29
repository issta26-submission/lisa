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
//<ID> 1725
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
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddNullToObject(config, "optional");

    // step 2: Configure
    cJSON_SetNumberHelper(threshold, 42.5);
    int nums[3] = {7, 8, 9};
    cJSON *values = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddStringToObject(root, "mode", "test");

    // step 3: Operate and Validate
    cJSON *retr_config = cJSON_GetObjectItem(root, "config");
    cJSON *retr_enabled = cJSON_GetObjectItem(retr_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(retr_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = is_enabled ? 'T' : 'F';
    double th_val = cJSON_GetNumberValue(threshold);
    buffer[2] = (char)('0' + ((int)th_val % 10));
    (void)buffer;
    (void)th_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}