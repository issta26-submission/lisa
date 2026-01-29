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
//<ID> 1571
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *direct_item = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    cJSON_bool added_array = 0;
    cJSON_bool added_from_ptr = 0;
    cJSON_bool is_direct_number = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    added_array = cJSON_AddItemToObject(root, "values", arr);
    direct_item = cJSON_AddNumberToObject(root, "direct", 3.1415);
    added_from_ptr = cJSON_AddItemToObject(root, "from_ptr", num1);
    cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    is_direct_number = cJSON_IsNumber(direct_item);
    dup_item = cJSON_Duplicate(cJSON_GetObjectItem(root, "from_ptr"), 1);

    // step 5: Validate
    validation_score = (int)added_array
        + (int)added_from_ptr
        + (int)is_direct_number
        + (int)(array_size == 1)
        + (int)(dup_item != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(dup_item);
    cJSON_Delete(root);
    (void)validation_score;
    (void)direct_item;
    (void)num2;
    (void)num1;
    (void)arr;
    (void)added_array;
    (void)added_from_ptr;
    // API sequence test completed successfully
    return 66;
}