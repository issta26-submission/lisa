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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[] = {1, 2, 3, 4};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObjectCS(root, "numbers", int_array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *to_replace = cJSON_CreateString("will_be_replaced");
    cJSON_AddItemToObjectCS(meta, "replace_me", to_replace);
    cJSON *replacement = cJSON_CreateString("I am replacement");
    cJSON_ReplaceItemViaPointer(meta, to_replace, replacement);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(meta, "nullable", null_item);

    // step 3: Operate and Validate
    int new_nums[] = {9, 8};
    cJSON *new_array = cJSON_CreateIntArray(new_nums, 2);
    cJSON_ReplaceItemViaPointer(root, int_array, new_array);
    cJSON *numbers_node = cJSON_GetObjectItem(root, "numbers");
    int numbers_count = cJSON_GetArraySize(numbers_node);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Cleanup
    (void)numbers_count;
    (void)prealloc_ok;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}