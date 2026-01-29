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
//<ID> 1384
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
    cJSON *false_item_in_array = (cJSON *)0;
    cJSON *status_false = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    int arr_size = 0;
    int is_status_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("element_one");
    false_item_in_array = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_item_in_array);

    // step 3: Configure - attach the array to the root and add a separate false status member
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "status", cJSON_CreateFalse());

    // step 4: Operate - retrieve members and inspect them
    found_arr = cJSON_GetObjectItem(root, "items");
    status_false = cJSON_GetObjectItem(root, "status");
    arr_size = cJSON_GetArraySize(found_arr);
    is_status_false = (int)cJSON_IsFalse(status_false);

    // step 5: Validate - compute a simple validation score
    validation_score = (int)(root != (cJSON *)0) + (int)(arr_size == 2) + is_status_false;

    // step 6: Cleanup - free the constructed cJSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)str_item;
    (void)false_item_in_array;
    return 66;
}