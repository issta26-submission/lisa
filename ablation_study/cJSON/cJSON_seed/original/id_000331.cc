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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums1[] = {1, 2, 3};
    cJSON *arr1 = cJSON_CreateIntArray(nums1, 3);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "numbers", arr1);
    cJSON_AddItemToObjectCS(root, "maybe_null", nullable);

    // step 2: Configure
    int nums2[] = {4, 5};
    cJSON *arr2 = cJSON_CreateIntArray(nums2, 2);
    cJSON *numbers_ptr = cJSON_GetObjectItem(root, "numbers");
    cJSON_ReplaceItemViaPointer(root, numbers_ptr, arr2);

    // step 3: Operate and Validate
    cJSON *numbers_new = cJSON_GetObjectItem(root, "numbers");
    int new_count = cJSON_GetArraySize(numbers_new);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *count_item = cJSON_CreateNumber((double)new_count);
    cJSON_AddItemToObjectCS(meta, "numbers_count", count_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON *prealloc_flag = cJSON_CreateBool(prealloc_ok);
    cJSON_AddItemToObjectCS(meta, "printed_prealloc", prealloc_flag);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}