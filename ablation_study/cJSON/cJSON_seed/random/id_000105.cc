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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *nums = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *retrieved_inner = (cJSON *)0;
    cJSON *retrieved_nums = (cJSON *)0;
    int nums_size = 0;
    cJSON *size_item = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *check_inner = (cJSON *)0;
    cJSON *check_count = (cJSON *)0;

    // step 2: Setup - create structure and populate array
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    nums = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToArray(nums, n3);
    cJSON_AddItemToObject(inner, "nums", nums);
    cJSON_AddItemToObject(root, "inner", inner);

    // step 3: Operate - query, record size, replace and detach
    retrieved_inner = cJSON_GetObjectItem(root, "inner");
    retrieved_nums = cJSON_GetObjectItem(retrieved_inner, "nums");
    nums_size = cJSON_GetArraySize(retrieved_nums);
    size_item = cJSON_CreateNumber((double)nums_size);
    cJSON_AddItemToObject(retrieved_inner, "nums_count", size_item);
    replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(retrieved_inner, "nums", replacement);
    detached = cJSON_DetachItemFromObject(retrieved_inner, "nums");

    // step 4: Validate - ensure count stayed attached to inner
    check_inner = cJSON_GetObjectItem(root, "inner");
    check_count = cJSON_GetObjectItem(check_inner, "nums_count");

    // step 5: Cleanup - free detached replacement and entire tree
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // step 6: API sequence test completed successfully
    return 66;
}