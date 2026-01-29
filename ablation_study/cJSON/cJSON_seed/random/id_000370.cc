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
//<ID> 370
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
    cJSON *got_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const double numbers[3] = {1.5, 2.5, 3.5};
    double value0 = 0.0;
    double value_detached = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(numbers, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "values");
    elem0 = cJSON_GetArrayItem(got_arr, 0);
    detached = cJSON_DetachItemFromArray(got_arr, 1);

    // step 5: Validate
    value0 = cJSON_GetNumberValue(elem0);
    value_detached = cJSON_GetNumberValue(detached);
    (void)value0;
    (void)value_detached;
    (void)got_arr;
    (void)elem0;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}