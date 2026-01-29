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
//<ID> 6
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

    // step 2: Configure and Operate
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_AddItemToArray(arr, str);
    cJSON_bool is_bool_true = cJSON_IsBool(btrue);
    cJSON_AddBoolToObject(root, "flag", is_bool_true);
    char *set_ret = cJSON_SetValuestring(str, "updated");

    // step 3: Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    int combined = first_is_bool + flag_is_bool * 2;
    (void)combined;
    (void)set_ret;
    (void)bfalse; // ensured all created items referenced or accounted for

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}