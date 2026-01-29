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
//<ID> 376
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
    cJSON *item_at_1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double values[3] = {1.25, 2.5, 3.75};
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(values, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "values");
    item_at_1 = cJSON_GetArrayItem(got_arr, 1);
    detached = cJSON_DetachItemFromArray(got_arr, 1);
    extracted_value = cJSON_GetNumberValue(detached);

    // step 5: Validate
    (void)item_at_1;
    (void)extracted_value;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}