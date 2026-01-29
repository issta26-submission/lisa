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
//<ID> 1610
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
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool added_arr_to_root = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.23);
    n2 = cJSON_CreateNumber(4.56);

    // step 3: Configure
    added_n1 = cJSON_AddItemToArray(arr, n1);
    added_n2 = cJSON_AddItemToArray(arr, n2);
    added_arr_to_root = cJSON_AddItemToObject(root, "myArray", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)added_n1
        + (int)added_n2
        + (int)added_arr_to_root
        + (int)(size_before == 2)
        + (int)(size_after == 1)
        + (int)(root != (cJSON *)0)
        + (int)(arr != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}