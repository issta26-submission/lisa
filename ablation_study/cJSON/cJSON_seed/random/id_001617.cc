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
//<ID> 1617
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(10.0);
    item1 = cJSON_CreateNumber(20.0);
    item2 = cJSON_CreateNumber(30.0);

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)added0
        + (int)added1
        + (int)added2
        + (int)added_arr
        + (int)(size_before == 3)
        + (int)(size_after == 2)
        + (int)(root != (cJSON *)0)
        + (int)(arr != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)item0;
    (void)item1;
    (void)item2;
    // API sequence test completed successfully
    return 66;
}