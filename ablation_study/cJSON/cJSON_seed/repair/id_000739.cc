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
//<ID> 739
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
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "stats", stats);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(stats, "count", count);
    cJSON *temp = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(stats, "temp", temp);

    // step 2: Configure
    cJSON *stats_dup = cJSON_Duplicate(stats, 1);
    cJSON_AddItemToObject(root, "stats_copy", stats_dup);
    cJSON *count_backup = cJSON_Duplicate(count, 0);
    cJSON_AddItemToObject(stats_dup, "count_backup", count_backup);

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *stats_ref = cJSON_GetObjectItem(meta_ref, "stats");
    cJSON *temp_ref = cJSON_GetObjectItem(stats_ref, "temp");
    cJSON *new_temp = cJSON_CreateNumber(6.28);
    cJSON_ReplaceItemViaPointer(stats_ref, temp_ref, new_temp);
    cJSON *count_ref = cJSON_GetObjectItem(stats_ref, "count");
    cJSON *count_dup = cJSON_Duplicate(count_ref, 0);
    cJSON_AddItemToObject(root, "count_copy", count_dup);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}