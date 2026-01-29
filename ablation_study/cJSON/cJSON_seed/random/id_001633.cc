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
//<ID> 1633
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
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    cJSON_bool added_greeting = 0;
    cJSON_bool added_list = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("first");
    str2 = cJSON_CreateString("second");

    // step 3: Configure
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_str2 = cJSON_AddItemToArray(arr, str2);
    added_list = cJSON_AddItemToObject(root, "items", arr);
    added_greeting = cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool greeting_is_string = cJSON_IsString(retrieved_greeting);

    // step 5: Validate
    validation_score = (int)added_str1
        + (int)added_str2
        + (int)added_list
        + (int)added_greeting
        + (int)(num_item != (cJSON *)0)
        + (int)greeting_is_string
        + (int)(array_size == 2);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_greeting;
    // API sequence test completed successfully
    return 66;
}