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
//<ID> 1382
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
    cJSON *retrieved = NULL;
    cJSON *dup_arr = NULL;
    int *nums = NULL;
    char *printed = NULL;
    cJSON_bool is_array = 0;
    cJSON_bool cmp_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 4);
    memset(nums, 0, sizeof(int) * 4);
    nums[0] = 1;
    nums[1] = 2;
    nums[2] = 3;
    nums[3] = 4;
    arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "data", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "data");
    is_array = cJSON_IsArray(retrieved);
    cmp_res = cJSON_Compare(retrieved, dup_arr, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)is_array;
    (void)cmp_res;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(nums);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}