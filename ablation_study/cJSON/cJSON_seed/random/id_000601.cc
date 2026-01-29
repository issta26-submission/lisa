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
//<ID> 601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr_a = (cJSON *)0;
    cJSON *arr_b = (cJSON *)0;
    cJSON *master = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *subitem = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    const int nums1[] = {10, 20, 30};
    const int nums2[] = {10, 20, 30};
    double extracted = 0.0;
    cJSON_bool cmp = 0;
    cJSON_bool added = 0;

    // step 2: Initialize
    arr_a = cJSON_CreateIntArray(nums1, 3);
    arr_b = cJSON_CreateIntArray(nums2, 3);
    master = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToArray(master, arr_a);
    cJSON_AddItemToArray(master, arr_b);

    // step 4: Operate
    subitem = cJSON_GetArrayItem(master, 0);
    detached = cJSON_DetachItemFromArray(master, 0);
    cmp = cJSON_Compare(detached, arr_b, 1);
    retrieved_num = cJSON_GetArrayItem(detached, 1);
    extracted = cJSON_GetNumberValue(retrieved_num);
    added = cJSON_AddItemToArray(master, cJSON_CreateNumber(extracted));

    // step 5: Validate
    (void)subitem;
    (void)cmp;
    (void)retrieved_num;
    (void)extracted;
    (void)added;

    // step 6: Cleanup
    cJSON_Delete(master);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}