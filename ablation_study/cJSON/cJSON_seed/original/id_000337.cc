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
//<ID> 337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums1[] = {10, 20, 30};
    cJSON *arr = cJSON_CreateIntArray(nums1, 3);
    cJSON_AddItemToObjectCS(root, "numbers", arr);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "maybe_null", null_item);

    // step 2: Configure
    int nums2[] = {7, 8, 9, 10};
    cJSON *replacement = cJSON_CreateIntArray(nums2, 4);
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, arr, replacement);
    (void)replaced_ok;
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(replacement);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObjectCS(meta, "count", size_num);
    cJSON *first = cJSON_GetArrayItem(replacement, 0);
    double first_value = cJSON_GetNumberValue(first);
    cJSON *first_num_copy = cJSON_CreateNumber(first_value);
    cJSON_AddItemToObjectCS(meta, "first_value", first_num_copy);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}