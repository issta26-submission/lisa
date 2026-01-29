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
//<ID> 731
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
    cJSON *meta = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_restored = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_meta = 0;
    cJSON_bool added_back = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");
    meta = cJSON_CreateNumber(123.0);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, item1);
    added2 = cJSON_AddItemToArray(arr, item2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    added_meta = cJSON_AddItemToObject(root, "meta", meta);

    // step 4: Operate
    detached = cJSON_DetachItemFromObject(root, "items");
    added_back = cJSON_AddItemToObject(root, "restored", detached);
    retrieved_restored = cJSON_GetObjectItem(root, "restored");
    retrieved_meta = cJSON_GetObjectItem(root, "meta");

    // step 5: Validate
    validation_score = (int)added1 + (int)added2 + (int)added_arr_to_obj + (int)added_meta + (retrieved_restored != (cJSON *)0) + (int)cJSON_IsNumber(retrieved_meta);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}