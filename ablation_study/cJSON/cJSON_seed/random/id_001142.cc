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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *numbers_array = (cJSON *)0;
    cJSON *fetched_numbers = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *detached_elem = (cJSON *)0;
    char *label_str = (char *)0;
    double v_before_detach = 0.0;
    double v_after_detach = 0.0;
    int validation_score = 0;
    const int nums[3] = {10, 20, 30};

    // step 2: Setup - create root object and attach a string and an int array
    root = cJSON_CreateObject();
    label_item = cJSON_CreateString("example-label");
    cJSON_AddItemToObject(root, "label", label_item);
    numbers_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", numbers_array);

    // step 3: Configure - retrieve object members and array pointer
    fetched_numbers = cJSON_GetObjectItem(root, "numbers");
    fetched_elem1 = cJSON_GetArrayItem(fetched_numbers, 1);
    label_item = cJSON_GetObjectItem(root, "label");

    // step 4: Operate - read number value, detach the array element, read again from detached
    v_before_detach = cJSON_GetNumberValue(fetched_elem1);
    detached_elem = cJSON_DetachItemViaPointer(fetched_numbers, fetched_elem1);
    v_after_detach = cJSON_GetNumberValue(detached_elem);
    label_str = cJSON_GetStringValue(label_item);

    // step 5: Validate - compute a small validation score using comparisons
    validation_score = (int)(v_before_detach == 20.0) + (int)(v_after_detach == 20.0) + (int)(label_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete detached element and root (which frees remaining items)
    cJSON_Delete(detached_elem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}