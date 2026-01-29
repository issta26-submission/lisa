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
//<ID> 748
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    cJSON *count = cJSON_AddNumberToObject(stats, "count", 42.0);
    cJSON *temp = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(stats, "temp", temp);

    // step 3: Operate and Validate
    cJSON *stats_ref = cJSON_GetObjectItem(root, "stats");
    cJSON_bool stats_is_object = cJSON_IsObject(stats_ref);
    double count_value_before = cJSON_GetNumberValue(cJSON_GetObjectItem(stats_ref, "count"));
    cJSON *detached_count = cJSON_DetachItemFromObject(stats, "count");
    cJSON_AddItemToObject(root, "orphan_count", detached_count);
    double count_value_after = cJSON_GetNumberValue(detached_count);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}