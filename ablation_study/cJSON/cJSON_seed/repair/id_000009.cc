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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON *str = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON *obj_bool = cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "list", arr);
    char *newstr = cJSON_SetValuestring(str, "updated");

    // step 3: Operate and Validate
    cJSON_bool is_obj_bool = cJSON_IsBool(obj_bool);
    cJSON *first_in_arr = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_first_bool = cJSON_IsBool(first_in_arr);
    int validation_sum = (int)is_obj_bool + (int)is_first_bool + (newstr != NULL);
    cJSON_SetNumberHelper(obj_bool, (double)validation_sum);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}