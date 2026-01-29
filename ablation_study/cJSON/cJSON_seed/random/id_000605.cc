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
//<ID> 605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item_after_detach = (cJSON *)0;
    cJSON_bool compare_result = 0;
    int numbers[3] = {10, 20, 30};

    // step 2: Initialize
    arr = cJSON_CreateIntArray(numbers, 3);

    // step 3: Configure
    first_item = cJSON_GetArrayItem(arr, 0);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    second_item_after_detach = cJSON_GetArrayItem(arr, 1);
    compare_result = cJSON_Compare(detached, second_item_after_detach, 1);

    // step 5: Validate
    (void)first_item;
    (void)compare_result;
    (void)second_item_after_detach;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}