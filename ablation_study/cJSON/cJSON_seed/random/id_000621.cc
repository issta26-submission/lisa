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
//<ID> 621
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
    cJSON *false_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON_bool second_is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    retrieved_arr = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "data"), 0);
    second_is_false = cJSON_IsFalse(second);

    // step 5: Validate
    (void)first;
    (void)retrieved_arr;
    (void)second_is_false;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}