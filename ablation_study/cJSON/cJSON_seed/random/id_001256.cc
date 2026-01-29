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
//<ID> 1256
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
    cJSON *child_num = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool str_is_string = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root, an independent numeric child, an array and a string item
    root = cJSON_CreateObject();
    child_num = cJSON_CreateNumber(123.456);
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");

    // step 3: Configure - create an array reference to the numeric child and attach items to the array, then attach array to root
    arr_ref = cJSON_CreateArrayReference((const cJSON *)child_num);
    cJSON_AddItemToArray(arr, arr_ref);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate - inspect the string item and measure the array size
    str_is_string = cJSON_IsString((const cJSON *const)str_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to ensure flow
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(child_num != (cJSON *)0) + (int)(arr_ref != (cJSON *)0) + (int)str_is_string + arr_size;
    (void)validation_score;

    // step 6: Cleanup - delete the root (frees array and referenced items added to it) and delete the independent numeric child
    cJSON_Delete(root);
    cJSON_Delete(child_num);

    // API sequence test completed successfully
    return 66;
}