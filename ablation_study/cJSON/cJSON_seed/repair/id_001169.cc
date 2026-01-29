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
//<ID> 1169
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
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    cJSON_bool is_obj = cJSON_IsObject(detached);
    cJSON *detached_threshold = cJSON_GetObjectItem(detached, "threshold");
    double set_ret = cJSON_SetNumberHelper(detached_threshold, 99.5);
    cJSON_bool is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    double final_value = set_ret + (double)is_obj + (double)is_null;
    cJSON_AddNumberToObject(root, "final", final_value);
    cJSON_AddItemToObject(root, "config_restored", detached);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}