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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr1 = (cJSON *)0;
    cJSON *arr2 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *moved_check_item = (cJSON *)0;
    cJSON *flag_num = (cJSON *)0;
    int size_arr1_before = 0;
    int size_arr1_after = 0;
    cJSON_bool moved_is_string = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr1 = cJSON_CreateArray();
    arr2 = cJSON_CreateArray();
    str_item = cJSON_CreateString("moved_string");
    true_item = cJSON_CreateTrue();
    num_item = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToArray(arr1, str_item);
    cJSON_AddItemToArray(arr1, true_item);
    cJSON_AddItemToArray(arr2, num_item);
    cJSON_AddItemToObject(root, "first_array", arr1);
    cJSON_AddItemToObject(root, "second_array", arr2);

    // step 4: Operate
    size_arr1_before = cJSON_GetArraySize(arr1);
    detached = cJSON_DetachItemFromArray(arr1, 0);
    cJSON_AddItemToArray(arr2, detached);

    // step 5: Validate
    size_arr1_after = cJSON_GetArraySize(arr1);
    moved_check_item = cJSON_GetArrayItem(arr2, 1);
    moved_is_string = cJSON_IsString(moved_check_item);
    flag_num = cJSON_CreateNumber((double)moved_is_string);
    cJSON_AddItemToObject(root, "moved_was_string", flag_num);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}