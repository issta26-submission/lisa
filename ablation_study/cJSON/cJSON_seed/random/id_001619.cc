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
//<ID> 1619
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
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_to_root = 0;
    int initial_size = 0;
    int size_after_delete = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    added_to_root = cJSON_AddItemToObject(root, "letters", arr);

    // step 4: Operate
    initial_size = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    size_after_delete = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)added1
        + (int)added2
        + (int)added_to_root
        + (int)(initial_size == 2)
        + (int)(size_after_delete == 1);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}