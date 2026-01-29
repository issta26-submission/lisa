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
//<ID> 1388
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
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *size_item = (cJSON *)0;
    int arr_size = 0;
    int is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with mixed elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(3.14);
    str_item = cJSON_CreateString("ok");
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Configure - attach the array to the root object
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - retrieve the array, measure size, inspect the first element, and record size as a named item
    found_arr = cJSON_GetObjectItem(root, "data");
    arr_size = cJSON_GetArraySize(found_arr);
    first_elem = cJSON_GetArrayItem(found_arr, 0);
    is_false_flag = (int)cJSON_IsFalse(first_elem);
    size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "size", size_item);

    // step 5: Validate - compose a trivial validation score from prior operations
    validation_score = (int)(root != (cJSON *)0) + arr_size + is_false_flag + (int)(size_item != (cJSON *)0);

    // step 6: Cleanup - delete the whole tree to free allocated memory
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)first_elem;
    return 66;
}