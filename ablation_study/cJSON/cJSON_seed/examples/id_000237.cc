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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "inner_key", cJSON_CreateString("inner_value"));
    cJSON_AddItemToObject(root, "child", child);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);

    // step 3: Operate & Validate
    cJSON *fetched_child = cJSON_GetObjectItem(root, "child");
    cJSON *fetched_items = cJSON_GetObjectItem(root, "items");
    int items_count = cJSON_GetArraySize(fetched_items);
    cJSON_bool first_is_raw = cJSON_IsRaw(cJSON_GetArrayItem(fetched_items, 0));
    cJSON *count_num = cJSON_CreateNumber((double)items_count);
    cJSON_AddItemToObject(root, "items_count", count_num);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)first_is_raw);
    cJSON_AddItemToObject(root, "first_is_raw", raw_flag_num);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_child_fetch = cJSON_GetObjectItem(dup_root, "child_copy");

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}