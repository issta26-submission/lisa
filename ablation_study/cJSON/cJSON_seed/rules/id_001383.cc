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
//<ID> 1383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *retrieved = NULL;
    cJSON *dup_item = NULL;
    int *nums = NULL;
    cJSON_bool is_arr = 0;
    cJSON_bool cmp_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 3);
    nums[0] = 1;
    nums[1] = 2;
    nums[2] = 3;
    arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "numbers");
    is_arr = cJSON_IsArray(retrieved);
    dup_item = cJSON_Duplicate(retrieved, 1);
    cmp_res = cJSON_Compare(retrieved, dup_item, 1);
    (void)is_arr;
    (void)cmp_res;

    // step 4: Cleanup
    cJSON_free(nums);
    cJSON_Delete(dup_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}