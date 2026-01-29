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
//<ID> 743
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_to_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_obj = cJSON_AddItemToObject(root, "object", obj);
    added_arr = cJSON_AddItemToObject(root, "array", arr);
    added_to_array = cJSON_AddItemToArray(arr, false_item);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    retrieved_arr = cJSON_GetObjectItem(root, "array");
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    validation_score = (retrieved_flag != (cJSON *)0) + (arr_size == 1) + (int)added_obj + (int)added_arr + (int)added_to_array + (flag_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}