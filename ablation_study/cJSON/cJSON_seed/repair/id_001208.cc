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
//<ID> 1208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *active = cJSON_CreateBool(1);
    cJSON *threshold = cJSON_CreateNumber(12.5);
    cJSON_AddItemToObject(settings, "name", name);
    cJSON_AddItemToObject(settings, "active", active);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *stats = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON *average = cJSON_CreateNumber(7.7);
    cJSON_AddItemToObject(stats, "count", count);
    cJSON_AddItemToObject(stats, "average", average);
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_null);

    // step 3: Operate and Validate
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_name = cJSON_GetObjectItem(got_settings, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_threshold = cJSON_GetObjectItem(got_settings, "threshold");
    double thr = cJSON_GetNumberValue(got_threshold);
    cJSON *got_stats = cJSON_GetObjectItem(root, "stats");
    cJSON *got_count = cJSON_GetObjectItem(got_stats, "count");
    double cnt = cJSON_GetNumberValue(got_count);
    cJSON_bool was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    double final_val = thr + cnt + (double)was_null;
    cJSON *final_item = cJSON_CreateNumber(final_val);
    cJSON_AddItemToObject(root, "final", final_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}