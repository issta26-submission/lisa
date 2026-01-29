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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *raw_fragment = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_fragment", raw_fragment);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "num", num);

    // step 2: Configure
    int initial_count = cJSON_GetArraySize(ints);
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double num_value = cJSON_GetNumberValue(num_item);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw_fragment");
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddNumberToObject(stats, "initial_count", (double)initial_count);
    cJSON_AddNumberToObject(stats, "num_value", num_value);
    cJSON_AddNumberToObject(stats, "is_raw", (double)is_raw);

    // step 3: Operate
    cJSON *added_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(ints, added_num);
    int new_count = cJSON_GetArraySize(ints);
    cJSON *last_item = cJSON_GetArrayItem(ints, new_count - 1);
    double last_value = cJSON_GetNumberValue(last_item);
    cJSON_AddNumberToObject(stats, "new_count", (double)new_count);
    cJSON_AddNumberToObject(stats, "last_value", last_value);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_AddNumberToObject(stats, "prealloc_ok", (double)prealloc_ok);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}