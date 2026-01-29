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
//<ID> 1384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *item0 = NULL;
    cJSON *item0_dup = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    int *nums = NULL;
    cJSON_bool is_array = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = (int *)cJSON_malloc(sizeof(int) * 3);
    nums[0] = 7;
    nums[1] = 11;
    nums[2] = 13;
    items = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNumberToObject(root, "total", 3.0);
    cJSON_AddStringToObject(root, "label", "primes");

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "items");
    is_array = cJSON_IsArray(retrieved);
    item0 = cJSON_GetArrayItem(retrieved, 0);
    item0_dup = cJSON_Duplicate(item0, 1);
    cmp_result = cJSON_Compare(item0, item0_dup, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)is_array;
    (void)cmp_result;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(nums);
    cJSON_free(printed);
    cJSON_Delete(item0_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}