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
//<ID> 1536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "app", "fuzz_client");
    cJSON *threshold_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "threshold", threshold_item);
    cJSON *enabled_item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_item);
    cJSON *state_item = cJSON_AddStringToObject(root, "state", "initialized");

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *retries_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "retries", retries_item);
    cJSON *mode_item = cJSON_AddStringToObject(config, "mode", "safe");

    // step 3: Operate and Validate
    double threshold_val = cJSON_GetNumberValue(threshold_item);
    double retries_val = cJSON_GetNumberValue(retries_item);
    const char *state_val = cJSON_GetStringValue(state_item);
    size_t buf_size = (size_t)64;
    char *scratch = (char *)cJSON_malloc(buf_size);
    memset(scratch, 0, buf_size);
    scratch[0] = (char)('0' + ((int)threshold_val % 10));
    scratch[1] = (char)('0' + ((int)retries_val % 10));
    scratch[2] = state_val[0];
    scratch[3] = name_item && name_item->string && name_item->string[0] ? name_item->string[0] : 'x';
    scratch[4] = (char)('#');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}