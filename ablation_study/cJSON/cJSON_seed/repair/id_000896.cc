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
//<ID> 896
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
    cJSON *items = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str_item);
    cJSON *active_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active_flag);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *third = cJSON_GetArrayItem(items, 2);
    const char *third_str = cJSON_GetStringValue(third);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root_copy, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}