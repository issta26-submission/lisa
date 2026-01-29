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
//<ID> 1381
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *item3 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_second = (cJSON *)0;
    int arr_size = 0;
    int is_second_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with three items (one false)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("first");
    item2 = cJSON_CreateFalse();
    item3 = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToArray(arr, item3);

    // step 3: Configure - attach the array to the root object under the key "items"
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve the array, measure its size, and inspect the second element for false
    found_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(found_arr);
    found_second = cJSON_GetArrayItem(found_arr, 1);
    is_second_false = (int)cJSON_IsFalse(found_second);

    // step 5: Validate - compute a simple validation score
    validation_score = (int)(root != (cJSON *)0) + (int)(arr_size == 3) + is_second_false;
    (void)validation_score;
    (void)found_second;
    (void)item1;
    (void)item2;
    (void)item3;

    // step 6: Cleanup - free the whole structure
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}