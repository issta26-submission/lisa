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
//<ID> 1397
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *picked_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    double detached_value = 0.0;
    int arr_size = 0;
    int is_number_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    n3 = cJSON_CreateNumber(3.5);

    // step 3: Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - pick the middle element, detach it from the array and re-add its value to root
    picked_item = cJSON_GetArrayItem(arr, 1);
    detached_item = cJSON_DetachItemViaPointer(arr, picked_item);
    detached_value = cJSON_GetNumberValue(detached_item);
    added_num = cJSON_AddNumberToObject(root, "detached", detached_value);

    // step 5: Validate - check array size decreased and new number exists
    arr_size = cJSON_GetArraySize(arr);
    is_number_flag = (int)cJSON_IsNumber(added_num);
    validation_score = arr_size + is_number_flag + (int)(detached_item != (cJSON *)0);

    // step 6: Cleanup - free detached standalone item and the remaining tree
    cJSON_Delete(detached_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)arr_size;
    (void)is_number_flag;
    (void)picked_item;
    return 66;
}