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
//<ID> 742
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "initial", 100.0);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 3: Operate and Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double doubled = count_val * 2.0;
    cJSON_AddNumberToObject(root, "double_count", doubled);
    cJSON *detached = cJSON_DetachItemFromObject(root, "count");
    cJSON *holder = cJSON_CreateObject();
    cJSON_AddItemToObject(holder, "moved_count", detached);
    cJSON_bool is_root_obj = cJSON_IsObject(root);
    cJSON_bool is_holder_obj = cJSON_IsObject(holder);
    cJSON_AddNumberToObject(root, "object_flags", (double)is_root_obj + (double)is_holder_obj);

    // step 4: Cleanup
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_holder = cJSON_PrintUnformatted(holder);
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_holder);
    cJSON_Delete(root);
    cJSON_Delete(holder);
    // API sequence test completed successfully
    return 66;
}