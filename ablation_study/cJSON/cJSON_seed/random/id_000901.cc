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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *flag_key = "flag";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool added_flag = (cJSON_bool)0;
    cJSON_bool first_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - populate array and attach to root, add a separate flag item
    cJSON_AddItemToArray(arr, cJSON_CreateFalse());
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    flag_item = cJSON_CreateFalse();
    added_flag = cJSON_AddItemToObject(root, flag_key, flag_item);

    // step 4: Operate - retrieve array items and perform checks
    first_elem = cJSON_GetArrayItem(arr, 0);
    second_elem = cJSON_GetArrayItem(arr, 1);
    first_is_false = cJSON_IsFalse(first_elem);
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)added_flag + (int)first_is_false + (int)(second_elem != (cJSON *)0);
    (void)validation_score;

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}