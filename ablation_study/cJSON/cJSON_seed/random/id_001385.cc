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
//<ID> 1385
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    int arr_size = 0;
    int flag_is_false = 0;
    int validation_score = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool addedarr = 0;
    cJSON_bool addedflag = 0;

    // step 2: Initialize - create root object, an array, two strings and a false boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate the array and attach members to root
    added1 = cJSON_AddItemToArray(arr, s1);
    added2 = cJSON_AddItemToArray(arr, s2);
    addedarr = cJSON_AddItemToObject(root, "numbers", arr);
    addedflag = cJSON_AddItemToObject(root, "enabled", flag_false);

    // step 4: Operate - retrieve the array and flag, measure array size and test false
    found_arr = cJSON_GetObjectItem(root, "numbers");
    found_flag = cJSON_GetObjectItem(root, "enabled");
    arr_size = cJSON_GetArraySize(found_arr);
    flag_is_false = (int)cJSON_IsFalse(found_flag);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + arr_size + flag_is_false + (int)added1 + (int)added2 + (int)addedarr + (int)addedflag;

    // step 6: Cleanup - delete the root which frees owned children
    cJSON_Delete(root);
    (void)found_arr;
    (void)found_flag;
    (void)validation_score;

    // API sequence test completed successfully
    return 66;
}