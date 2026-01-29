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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *flag_key = "flag";
    const char *num_key = "num";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool added_flag = (cJSON_bool)0;
    cJSON_bool added_num = (cJSON_bool)0;
    cJSON_bool is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    flag = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(7.0);

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);
    added_flag = cJSON_AddItemToObject(root, flag_key, flag);
    added_num = cJSON_AddItemToObject(root, num_key, num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 4: Operate / Validate
    first_elem = cJSON_GetArrayItem(arr, 0);
    retrieved_flag = cJSON_GetObjectItem(root, flag_key);
    is_false = cJSON_IsFalse(retrieved_flag);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(first_elem != (cJSON *)0) + (int)is_false + (int)added_arr + (int)added_flag + (int)added_num + (int)(num_item != (cJSON *)0);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}