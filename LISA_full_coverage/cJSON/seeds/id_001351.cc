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
//<ID> 1351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *settings = cJSON_AddObjectToObject(config, "settings");
    float thresholds[3] = {1.25f, 2.50f, 3.75f};
    cJSON *float_arr = cJSON_CreateFloatArray(thresholds, 3);
    cJSON_AddItemToObject(settings, "thresholds", float_arr);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(settings, "optional", optional_null);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    const char *sample_json = "{\"keep\":42,\"name\":\"demo\"}";
    cJSON *parsed = cJSON_Parse(sample_json);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *set = cJSON_GetObjectItem(cfg, "settings");
    cJSON *arr_item = cJSON_GetObjectItem(set, "thresholds");
    int count = cJSON_GetArraySize(arr_item);
    cJSON *count_marker = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "threshold_count", count_marker);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}