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
//<ID> 908
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *bool_key = "enabled";
    double value0 = 1.0;
    double value1 = 2.0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON_bool added_num0 = (cJSON_bool)0;
    cJSON_bool added_num1 = (cJSON_bool)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool added_false = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(value0);
    num1 = cJSON_CreateNumber(value1);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_num0 = cJSON_AddItemToArray(arr, num0);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    added_false = cJSON_AddItemToObject(root, bool_key, false_item);

    // step 4: Operate / Validate
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, bool_key));
    validation_score = (int)(root != (cJSON *)0) + (int)added_arr + (int)added_false + (int)(retrieved0 != (cJSON *)0) + (int)is_false + (int)added_num0 + (int)added_num1;
    (void)validation_score;

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}