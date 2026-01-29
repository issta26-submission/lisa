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
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const int nums1[] = {1, 2, 3};
    const int nums2[] = {1, 2, 3};
    cJSON *arr1 = (cJSON *)0;
    cJSON *arr2 = (cJSON *)0;
    cJSON *item_from_arr1 = (cJSON *)0;
    cJSON *detached_from_arr2 = (cJSON *)0;
    cJSON *post_item_arr2 = (cJSON *)0;
    cJSON_bool are_equal = 0;
    int size_arr1 = 0;
    int size_arr2 = 0;

    // step 2: Initialize
    arr1 = cJSON_CreateIntArray(nums1, 3);
    arr2 = cJSON_CreateIntArray(nums2, 3);

    // step 3: Configure
    item_from_arr1 = cJSON_GetArrayItem(arr1, 2);
    detached_from_arr2 = cJSON_DetachItemFromArray(arr2, 2);

    // step 4: Operate
    are_equal = cJSON_Compare(item_from_arr1, detached_from_arr2, 1);

    // step 5: Validate
    size_arr1 = cJSON_GetArraySize(arr1);
    size_arr2 = cJSON_GetArraySize(arr2);
    post_item_arr2 = cJSON_GetArrayItem(arr2, 1);
    (void)size_arr1;
    (void)size_arr2;
    (void)post_item_arr2;
    (void)are_equal;

    // step 6: Cleanup
    cJSON_Delete(arr1);
    cJSON_Delete(arr2);
    cJSON_Delete(detached_from_arr2);

    // API sequence test completed successfully
    return 66;
}