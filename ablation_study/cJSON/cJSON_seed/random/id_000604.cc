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
//<ID> 604
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item_second = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const int nums[4] = {10, 20, 30, 40};
    int count = 4;
    cJSON_bool cmp_result = 0;
    double first_value = 0.0;
    cJSON *tmp = (cJSON *)0;

    // step 2: Initialize
    arr = cJSON_CreateIntArray(nums, count);
    root = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    item_second = cJSON_GetArrayItem(arr, 1);
    detached = cJSON_DetachItemFromArray(arr, 1);
    cmp_result = cJSON_Compare(detached, item_second, (cJSON_bool)1);
    tmp = cJSON_AddNumberToObject(root, "compare_equal", (double)cmp_result);

    // step 5: Validate
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_value", first_value);
    (void)tmp;
    (void)first_value;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}