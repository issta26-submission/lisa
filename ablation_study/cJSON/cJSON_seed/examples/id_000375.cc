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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *num = cJSON_CreateNumber(7.5);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON *arr = cJSON_CreateArray();
    cJSON *dup_greeting = cJSON_Duplicate(greeting, 1);
    cJSON_AddItemToArray(arr, dup_greeting);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.5));
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "maybe", maybe_missing);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    double base_value = cJSON_GetNumberValue(num);
    double increment = 10.0;
    cJSON *sum = cJSON_CreateNumber(base_value + increment);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    char *meta_buf = (char *)cJSON_malloc(128);
    memset(meta_buf, 0, 128);
    sprintf(meta_buf, "was_null=%d;base=%.2f", (int)was_null, base_value);
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_free(meta_buf);

    // step 3: Operate & Validate
    cJSON *second_in_list = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_in_list);
    double total = base_value + second_val + increment;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);

    // step 4: Cleanup & finalize
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}