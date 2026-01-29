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
//<ID> 1387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *ints_arr = NULL;
    cJSON *meta = NULL;
    cJSON *name_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *true_ref = NULL;
    cJSON *retrieved_ints = NULL;
    cJSON *retrieved_flag = NULL;
    char *printed = NULL;
    int *nums = NULL;
    cJSON_bool is_array = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 4);
    nums[0] = 7;
    nums[1] = 14;
    nums[2] = 21;
    nums[3] = 28;
    ints_arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", ints_arr);
    meta = cJSON_CreateObject();
    name_item = cJSON_CreateString("sample");
    cJSON_AddItemToObject(meta, "name", name_item);
    bool_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "flag", bool_item);
    cJSON_AddItemToObject(root, "meta", meta);
    true_ref = cJSON_CreateTrue();

    // step 3: Operate / Validate
    retrieved_ints = cJSON_GetObjectItem(root, "ints");
    is_array = cJSON_IsArray(retrieved_ints);
    retrieved_flag = cJSON_GetObjectItem(meta, "flag");
    cmp_result = cJSON_Compare(retrieved_flag, true_ref, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)is_array;
    (void)cmp_result;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(nums);
    cJSON_Delete(true_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}