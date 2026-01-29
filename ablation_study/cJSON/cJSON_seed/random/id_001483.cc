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
//<ID> 1483
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
    cJSON *label = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    char *printed = (char *)0;
    const char *label_str = (const char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool is_raw_0 = 0;
    cJSON_bool is_raw_1 = 0;
    float nums[3];
    memset(nums, 0, sizeof(nums));

    // step 2: Initialize - prepare float data and create JSON nodes
    nums[0] = 1.1f;
    nums[1] = 2.2f;
    nums[2] = 3.3f;
    arr = cJSON_CreateFloatArray(nums, 3);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "values", arr);
    label = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate - inspect array and items
    arr_size = cJSON_GetArraySize(arr);
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    is_raw_0 = cJSON_IsRaw(elem0);
    is_raw_1 = cJSON_IsRaw(elem1);
    label_str = cJSON_GetStringValue(label);

    // step 4: Validate - create a printed representation and derive a score
    printed = cJSON_PrintUnformatted(root);
    validation_score = arr_size + (int)is_raw_0 + (int)is_raw_1 + (int)(label_str != (const char *)0) + (int)(printed != (char *)0);

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)arr_size;
    (void)elem0;
    (void)elem1;
    (void)label_str;
    (void)is_raw_0;
    (void)is_raw_1;
    (void)nums;
    // API sequence test completed successfully
    return 66;
}