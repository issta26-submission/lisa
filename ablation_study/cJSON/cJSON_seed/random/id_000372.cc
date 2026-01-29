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
//<ID> 372
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
    cJSON *got_array = (cJSON *)0;
    cJSON *item_before = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const double numbers[3] = {1.5, 2.5, 3.5};
    int index = 1;
    double value_before = 0.0;
    double value_detached = 0.0;
    double combined_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(numbers, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    got_array = cJSON_GetObjectItem(root, "values");
    item_before = cJSON_GetArrayItem(got_array, index);
    detached = cJSON_DetachItemFromArray(got_array, index);

    // step 5: Validate
    value_before = cJSON_GetNumberValue(item_before);
    value_detached = cJSON_GetNumberValue(detached);
    combined_value = value_before + value_detached;
    (void)combined_value;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}