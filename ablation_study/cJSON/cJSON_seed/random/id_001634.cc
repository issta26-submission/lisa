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
//<ID> 1634
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    cJSON *retrieved_number = (cJSON *)0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_greeting_to_root = 0;
    int array_size = 0;
    cJSON_bool is_string = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("one");
    str2 = cJSON_CreateString("two");
    greeting = cJSON_CreateString("hello");

    // step 3: Configure
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_str2 = cJSON_AddItemToArray(arr, str2);
    added_arr_to_root = cJSON_AddItemToObject(root, "list", arr);
    added_greeting_to_root = cJSON_AddItemToObject(root, "greeting", greeting);
    retrieved_number = cJSON_AddNumberToObject(root, "count", 42.0);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    is_string = cJSON_IsString(retrieved_greeting);

    // step 5: Validate
    validation_score = (int)added_str1
        + (int)added_str2
        + (int)added_arr_to_root
        + (int)added_greeting_to_root
        + (int)is_string
        + (int)(array_size == 2)
        + (int)(retrieved_number != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_greeting;
    (void)str1;
    (void)str2;
    (void)greeting;
    (void)added_str1;
    (void)added_str2;
    (void)added_arr_to_root;
    (void)added_greeting_to_root;
    (void)array_size;
    (void)is_string;
    (void)retrieved_number;
    // API sequence test completed successfully
    return 66;
}