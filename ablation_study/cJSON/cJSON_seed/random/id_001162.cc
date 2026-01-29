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
//<ID> 1162
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
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *fetched_flag = (cJSON *)0;
    cJSON_bool is_elem0_bool = 0;
    cJSON_bool is_elem1_bool = 0;
    cJSON_bool is_flag_bool = 0;
    int validation_score = 0;

    // step 2: Initialize / Configure - create root object, array and boolean items, attach to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, false_item);
    flag_item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate - retrieve the array and its elements and retrieve the flag member
    fetched_arr = cJSON_GetObjectItem(root, "data");
    elem0 = cJSON_GetArrayItem(fetched_arr, 0);
    elem1 = cJSON_GetArrayItem(fetched_arr, 1);
    fetched_flag = cJSON_GetObjectItem(root, "flag");

    // step 4: Validate - check that retrieved items are boolean and compute a small score
    is_elem0_bool = cJSON_IsBool(elem0);
    is_elem1_bool = cJSON_IsBool(elem1);
    is_flag_bool = cJSON_IsBool(fetched_flag);
    validation_score = (int)is_elem0_bool + (int)is_elem1_bool + (int)is_flag_bool;
    (void)validation_score;

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}