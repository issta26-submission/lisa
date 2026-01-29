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
//<ID> 1611
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
    cJSON_bool added_arr = 0;
    cJSON_bool added_item1 = 0;
    cJSON_bool added_item2 = 0;
    cJSON_bool added_item3 = 0;
    int size_before = 0;
    int size_after_delete = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateNumber(10.0);
    item2 = cJSON_CreateNumber(20.0);
    item3 = cJSON_CreateNumber(30.0);

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    added_item1 = cJSON_AddItemToArray(arr, item1);
    added_item2 = cJSON_AddItemToArray(arr, item2);
    added_item3 = cJSON_AddItemToArray(arr, item3);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 1);
    size_after_delete = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)added_arr
        + (int)added_item1
        + (int)added_item2
        + (int)added_item3
        + (int)(size_before == 3)
        + (int)(size_after_delete == 2);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)size_before;
    (void)size_after_delete;
    (void)added_item3;
    // API sequence test completed successfully
    return 66;
}