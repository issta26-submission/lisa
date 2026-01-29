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
//<ID> 607
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ints_a = (cJSON *)0;
    cJSON *ints_b = (cJSON *)0;
    cJSON *retr_a = (cJSON *)0;
    cJSON *retr_b = (cJSON *)0;
    cJSON *first_a = (cJSON *)0;
    cJSON *second_b = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON_bool are_equal = 0;
    int nums[3];
    int count = 3;

    // step 2: Initialize
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    root = cJSON_CreateObject();
    ints_a = cJSON_CreateIntArray(nums, count);
    ints_b = cJSON_CreateIntArray(nums, count);

    // step 3: Configure
    cJSON_AddItemToObject(root, "ints_a", ints_a);
    cJSON_AddItemToObject(root, "ints_b", ints_b);

    // step 4: Operate
    retr_a = cJSON_GetObjectItem(root, "ints_a");
    retr_b = cJSON_GetObjectItem(root, "ints_b");
    first_a = cJSON_GetArrayItem(retr_a, 0);
    second_b = cJSON_GetArrayItem(retr_b, 1);
    detached_item = cJSON_DetachItemFromArray(retr_a, 1);
    are_equal = cJSON_Compare(detached_item, second_b, 1);
    cJSON_AddBoolToObject(root, "detached_equals_b_second", are_equal);

    // step 5: Validate
    (void)first_a;
    (void)are_equal;
    (void)second_b;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_item);

    // API sequence test completed successfully
    return 66;
}