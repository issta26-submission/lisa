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
//<ID> 770
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
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("present");
    false_item = cJSON_CreateFalse();
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_false = cJSON_AddItemToArray(arr, false_item);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);

    // step 5: Validate
    validation_score = (retrieved != (cJSON *)0) + (int)is_false + (int)added_str + (int)added_false + (int)cJSON_IsArray(arr) + (int)cJSON_IsObject(root);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}