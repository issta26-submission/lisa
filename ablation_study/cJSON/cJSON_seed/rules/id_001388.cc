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
//<ID> 1388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *numbers_arr = NULL;
    cJSON *retrieved = NULL;
    cJSON *copied = NULL;
    char *printed = NULL;
    int *nums = NULL;
    cJSON_bool is_arr = 0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 4);
    nums[0] = 7;
    nums[1] = 14;
    nums[2] = 21;
    nums[3] = 28;
    numbers_arr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "numbers", numbers_arr);
    cJSON_AddStringToObject(root, "note", "sequence_test");

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "numbers");
    is_arr = cJSON_IsArray(retrieved);
    cmp = cJSON_Compare(numbers_arr, retrieved, 1);
    copied = cJSON_Duplicate(retrieved, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)is_arr;
    (void)cmp;

    // step 4: Cleanup
    cJSON_free(nums);
    cJSON_free(printed);
    cJSON_Delete(copied);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}