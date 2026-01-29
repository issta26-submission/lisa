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
//<ID> 1386
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
    cJSON *str_item = (cJSON *)0;
    cJSON *false_in_array = (cJSON *)0;
    cJSON *false_in_object = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    int arr_size = 0;
    int is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("one");
    false_in_array = cJSON_CreateFalse();
    false_in_object = cJSON_CreateFalse();

    // step 3: Configure - populate the array and attach array and flag to root object
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_in_array);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "is_active", false_in_object);

    // step 4: Operate - retrieve members and inspect them
    found_arr = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(found_arr);
    found_flag = cJSON_GetObjectItem(root, "is_active");
    is_false_flag = (int)cJSON_IsFalse(found_flag);

    // step 5: Validate - compute a simple validation score based on operations
    validation_score = (int)(found_arr != (cJSON *)0) + (arr_size == 2) + is_false_flag;

    // step 6: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)is_false_flag;
    (void)found_arr;
    (void)found_flag;
    return 66;
}