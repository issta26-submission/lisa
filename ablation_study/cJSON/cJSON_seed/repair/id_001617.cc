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
//<ID> 1617
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *first_num = cJSON_CreateNumber(10.0);
    cJSON *second_str = cJSON_CreateString("temporary");
    cJSON_AddItemToArray(arr, first_num);
    cJSON_AddItemToArray(arr, second_str);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *to_detach = cJSON_CreateString("detachable_value");
    cJSON_AddItemToObject(root, "to_detach", to_detach);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "meta_id", 7.0);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *placeholder = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "derived", placeholder);

    // step 3: Operate and Validate
    cJSON *got_first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(got_first);
    cJSON *double_first = cJSON_CreateNumber(first_val * 2.0);
    cJSON_ReplaceItemInObject(root, "derived", double_first);
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "to_detach");
    const char *detached_str = cJSON_GetStringValue(detached);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = detached_str[0];
    buffer[1] = (char)('0' + ((int)first_val % 10));

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}