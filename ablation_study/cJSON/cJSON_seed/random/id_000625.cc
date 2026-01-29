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
//<ID> 625
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_array = 0;
    cJSON_bool added_label = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(123.0);
    str_item = cJSON_CreateString("example");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_array = cJSON_AddItemToArray(arr, num_item);
    added_array = cJSON_AddItemToArray(arr, false_item);
    added_label = cJSON_AddItemToObject(root, "array", arr);
    (void)cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(retrieved);

    // step 5: Validate
    (void)added_array;
    (void)added_label;
    (void)retrieved;
    (void)is_false;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}