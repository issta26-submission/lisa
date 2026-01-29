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
//<ID> 335
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
    cJSON *arr = cJSON_CreateIntArray(nums1, 3);
    cJSON_AddItemToObjectCS(root, "ints", arr);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    int nums2[] = {42, 99};
    cJSON *arr2 = cJSON_CreateIntArray(nums2, 2);

    // step 3: Operate and Validate
    cJSON_bool replaced_to_null = cJSON_ReplaceItemViaPointer(root, arr, null_item);
    cJSON *after_first = cJSON_GetObjectItem(root, "ints");
    cJSON_bool replaced_to_array = cJSON_ReplaceItemViaPointer(root, after_first, arr2);
    int final_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "ints"));
    cJSON_AddBoolToObject(meta, "replaced_to_null", replaced_to_null);
    cJSON_AddBoolToObject(meta, "replaced_to_array", replaced_to_array);
    cJSON_AddNumberToObject(meta, "final_size", (double)final_size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}