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
//<ID> 627
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
    cJSON *false_in_array = (cJSON *)0;
    cJSON *false_in_object = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_in_array = cJSON_CreateFalse();
    false_in_object = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, false_in_array);
    cJSON_AddItemToObject(root, "flag_obj", false_in_object);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_item = cJSON_GetArrayItem(arr, 0);
    is_false = cJSON_IsFalse(retrieved_item);

    // step 5: Validate
    (void)is_false;
    (void)retrieved_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}