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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *arr = cJSON_CreateFloatArray(nums, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "source", "fuzz");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    int arr_count = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_num = cJSON_GetNumberValue(first_item);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 64, 1);
    int checksum = arr_count + (int)first_num + (int)root_is_obj + (int)meta_is_obj + (int)printed[0] + (int)buffered[0];
    (void)checksum;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}