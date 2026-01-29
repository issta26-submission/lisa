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
//<ID> 1630
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *elem3 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *retrieved_s1 = (cJSON *)0;
    cJSON *number_item = (cJSON *)0;
    cJSON_bool added_greetings = 0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    cJSON_bool added_number = 0;
    cJSON_bool added_arr_item = 0;
    cJSON_bool is_s1_string = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");
    elem3 = cJSON_CreateString("three");
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    added_greetings = cJSON_AddItemToObject(root, "greetings", arr);
    added_s1 = cJSON_AddItemToObject(root, "s1", str1);
    added_s2 = cJSON_AddItemToObject(root, "s2", str2);
    number_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    added_number = (number_item != (cJSON *)0);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    added_arr_item = cJSON_AddItemToArray(arr, elem3);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    retrieved_s1 = cJSON_GetObjectItem(root, "s1");
    is_s1_string = cJSON_IsString(retrieved_s1);
    validation_score = (int)added_greetings
        + (int)added_s1
        + (int)added_s2
        + (int)added_number
        + (int)added_arr_item
        + (int)(size_before == 2)
        + (int)(size_after == 3)
        + (int)is_s1_string;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_s1;
    (void)elem1;
    (void)elem2;
    (void)elem3;
    (void)str1;
    (void)str2;
    (void)number_item;
    // API sequence test completed successfully
    return 66;
}