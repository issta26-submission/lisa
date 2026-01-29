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
//<ID> 379
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
    cJSON *elem_before = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double values[3] = {1.1, 2.2, 3.3};
    double extracted_value = 0.0;
    int detach_index = 1;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(values, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "nums", arr);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "nums");
    elem_before = cJSON_GetArrayItem(got_arr, detach_index);
    detached = cJSON_DetachItemFromArray(got_arr, detach_index);

    // step 5: Validate
    extracted_value = cJSON_GetNumberValue(detached);
    (void)elem_before;
    (void)extracted_value;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}