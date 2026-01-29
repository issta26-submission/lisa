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
//<ID> 744
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

    // step 2: Configure
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 10.5);
    cJSON *offset = cJSON_AddNumberToObject(config, "offset", 2.25);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *version = cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate and Validate
    cJSON_bool is_obj_flag = cJSON_IsObject(config);
    double thresh_val = cJSON_GetNumberValue(threshold);
    double offset_val = cJSON_GetNumberValue(offset);
    double sum_val = thresh_val + offset_val + (double)is_obj_flag;
    cJSON *sum_node = cJSON_AddNumberToObject(root, "sum", sum_val);
    cJSON *detached_offset = cJSON_DetachItemFromObject(config, "offset");
    cJSON_AddItemToObject(root, "detached_offset", detached_offset);
    cJSON *sum_ref = cJSON_GetObjectItem(root, "sum");
    double retrieved_sum = cJSON_GetNumberValue(sum_ref);
    cJSON_AddNumberToObject(root, "retrieved_sum", retrieved_sum);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}