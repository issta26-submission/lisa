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
//<ID> 749
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *added_flag_item = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, false_item);
    added_flag_item = cJSON_AddBoolToObject(child, "enabled", 0);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_flag = cJSON_GetObjectItem(retrieved_child, "enabled");
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (retrieved_child != (cJSON *)0) + (retrieved_flag != (cJSON *)0) + (arr_size == 1);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}