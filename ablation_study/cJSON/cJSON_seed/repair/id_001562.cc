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
//<ID> 1562
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
    cJSON *item1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, item1);
    cJSON *item3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, item3);
    cJSON *item2 = cJSON_CreateString("two");
    cJSON_InsertItemInArray(arr, 1, item2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *desc = cJSON_CreateString("a description");
    cJSON_AddItemToObject(meta, "desc", desc);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON *desc_item = cJSON_GetObjectItem(detached_meta, "desc");
    const char *desc_str = cJSON_GetStringValue(desc_item);
    cJSON *middle_item = cJSON_GetArrayItem(arr, 1);
    const char *mid_str = cJSON_GetStringValue(middle_item);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    char *scratch = (char *)cJSON_malloc(printed_len + 16);
    memset(scratch, 0, printed_len + 16);
    scratch[0] = desc_str[0];
    scratch[1] = mid_str[0];
    scratch[2] = (char)('0' + (int)arr_size);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}