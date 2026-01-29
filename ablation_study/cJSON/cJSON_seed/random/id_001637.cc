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
//<ID> 1637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool added_list = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_elem1 = 0;
    cJSON_bool added_elem2 = 0;
    int arr_size = 0;
    cJSON_bool is_string = 0;
    cJSON_bool is_number = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");

    // step 3: Configure
    added_list = cJSON_AddItemToObject(root, "list", arr);
    added_elem1 = cJSON_AddItemToArray(arr, elem1);
    added_elem2 = cJSON_AddItemToArray(arr, elem2);
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);
    num_item = cJSON_AddNumberToObject(root, "id", 123.0);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    retrieved_str = cJSON_GetObjectItem(root, "greeting");
    is_string = cJSON_IsString(retrieved_str);
    retrieved_num = cJSON_GetObjectItem(root, "id");
    is_number = cJSON_IsNumber(retrieved_num);

    // step 5: Validate
    validation_score = (int)added_list
        + (int)added_elem1
        + (int)added_elem2
        + (int)added_str
        + (int)is_string
        + (int)is_number
        + (int)(arr_size == 2);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}