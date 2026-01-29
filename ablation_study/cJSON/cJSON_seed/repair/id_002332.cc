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
//<ID> 2332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *threshold = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(stats, "count", count);

    // step 2: Configure
    cJSON *thr_item = cJSON_GetObjectItem(settings, "threshold");
    cJSON *cnt_item = cJSON_GetObjectItem(stats, "count");
    double thr_val = cJSON_GetNumberValue(thr_item);
    double cnt_val = cJSON_GetNumberValue(cnt_item);
    cJSON *sum = cJSON_CreateNumber(thr_val + cnt_val);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 3: Operate
    cJSON *fetched_sum = cJSON_GetObjectItem(root, "sum");
    cJSON_bool sum_invalid = cJSON_IsInvalid(fetched_sum);
    char *json = cJSON_Print(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)(int)sum_invalid;
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}