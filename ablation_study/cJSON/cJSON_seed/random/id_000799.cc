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
//<ID> 799
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
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    cJSON *retrieved_true = (cJSON *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_true = 0;
    cJSON_bool added_to_obj = 0;
    cJSON_bool is_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_true = cJSON_AddItemToArray(arr, true_item);
    added_to_obj = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_str = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_true = cJSON_GetArrayItem(retrieved_arr, 1);
    is_true = cJSON_IsTrue(retrieved_true);

    // step 5: Validate
    validation_score = (retrieved_arr != (cJSON *)0) + (retrieved_str != (cJSON *)0) + (retrieved_true != (cJSON *)0) + (int)is_true + (int)added_str + (int)added_true + (int)added_to_obj;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}