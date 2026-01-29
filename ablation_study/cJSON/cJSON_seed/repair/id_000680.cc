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
//<ID> 680
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num_initial = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(cfg, "initial", num_initial);
    cJSON *num_api = cJSON_AddNumberToObject(cfg, "added_via_api", 3.14);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(cfg, "nothing", null_item);
    cJSON *count_item = cJSON_AddNumberToObject(meta, "count", 0.0);

    // step 2: Configure
    double set_ret = cJSON_SetNumberHelper(num_initial, 2.718);
    cJSON *last_set = cJSON_AddNumberToObject(meta, "last_set", set_ret);
    (void)last_set;
    (void)num_api;

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(cfg, num_initial);
    cJSON_AddItemToObject(meta, "moved_initial", detached);
    cJSON_SetNumberHelper(count_item, 2.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}