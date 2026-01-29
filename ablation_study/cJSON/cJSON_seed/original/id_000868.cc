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
//<ID> 868
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
    cJSON_AddItemToObject(root, "settings", settings);
    float thresholds[] = { 1.0f, 2.5f, -3.0f, 4.25f };
    cJSON *th_arr = cJSON_CreateFloatArray(thresholds, 4);
    cJSON_AddItemToObject(settings, "thresholds", th_arr);
    cJSON_AddFalseToObject(settings, "enabled");

    // step 2: Configure
    char json_buf[] = "  { \"meta\": { \"version\": 2.5 }, \"flag\": false }   ";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_buf, &parse_end, 1);

    // step 3: Operate
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    double version = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "version"));
    cJSON *ver_num = cJSON_CreateNumber(version);
    cJSON_AddItemToArray(th_arr, ver_num);
    int arr_size = cJSON_GetArraySize(th_arr);
    cJSON *count_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "threshold_count", count_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}