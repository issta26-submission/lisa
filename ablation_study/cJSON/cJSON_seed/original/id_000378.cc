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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *raw = cJSON_CreateRaw("{\"nested\":true}");
    cJSON_AddItemToObject(root, "raw_json", raw);

    // step 2: Configure
    int count = cJSON_GetArraySize(int_array);
    cJSON *middle_item = cJSON_GetArrayItem(int_array, 1);
    double middle_value = cJSON_GetNumberValue(middle_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddNumberToObject(stats, "count", (double)count);
    cJSON_AddNumberToObject(stats, "middle_value", middle_value);
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