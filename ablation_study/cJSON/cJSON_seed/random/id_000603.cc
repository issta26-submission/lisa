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
//<ID> 603
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *intarr = (cJSON *)0;
    cJSON *copy = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    int numbers[3] = {10, 20, 30};
    int count = 3;
    cJSON_bool cmp_before = 0;
    cJSON_bool cmp_after = 0;
    double first_val = 0.0;
    double second_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    intarr = cJSON_CreateIntArray(numbers, count);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", intarr);
    first_item = cJSON_GetArrayItem(intarr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    copy = cJSON_Duplicate(intarr, 1);
    cJSON_AddItemToObject(root, "numbers_copy", copy);

    // step 4: Operate
    second_item = cJSON_GetArrayItem(intarr, 1);
    second_val = cJSON_GetNumberValue(second_item);
    cmp_before = cJSON_Compare(intarr, copy, 1);
    detached = cJSON_DetachItemFromArray(intarr, 1);
    cmp_after = cJSON_Compare(intarr, copy, 1);

    // step 5: Validate
    (void)first_val;
    (void)second_val;
    (void)cmp_before;
    (void)cmp_after;
    (void)second_item;
    (void)first_item;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}