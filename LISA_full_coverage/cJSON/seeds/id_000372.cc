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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[5] = {1, 2, 3, 4, 5};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(nums, 5);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *raw_item = cJSON_CreateRaw("{\"note\":\"raw content\"}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    int count = cJSON_GetArraySize(ints);
    cJSON *third = cJSON_GetArrayItem(ints, 2);
    double third_value = cJSON_GetNumberValue(third);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddNumberToObject(stats, "count", (double)count);
    cJSON_AddNumberToObject(stats, "third_value", third_value);
    cJSON_AddNumberToObject(stats, "raw_is_raw", (double)raw_is_raw);

    // step 3: Operate
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