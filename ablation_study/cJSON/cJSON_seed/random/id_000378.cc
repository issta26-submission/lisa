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
//<ID> 378
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
    cJSON *container = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *item_before = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *got_taken = (cJSON *)0;
    double numbers[3];
    double value_before = 0.0;
    double value_after = 0.0;

    // step 2: Initialize
    numbers[0] = 1.5;
    numbers[1] = 2.5;
    numbers[2] = 3.5;
    arr = cJSON_CreateDoubleArray(numbers, 3);
    root = cJSON_CreateObject();
    container = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "numbers");
    item_before = cJSON_GetArrayItem(got_arr, 1);
    detached = cJSON_DetachItemFromArray(got_arr, 1);
    cJSON_AddItemToObject(container, "taken", detached);
    got_taken = cJSON_GetObjectItem(container, "taken");

    // step 5: Validate
    value_before = cJSON_GetNumberValue(item_before);
    value_after = cJSON_GetNumberValue(got_taken);
    (void)value_before;
    (void)value_after;
    (void)got_arr;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(container);

    // API sequence test completed successfully
    return 66;
}