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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations (Initialize)
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *added_flag = cJSON_AddBoolToObject(root, "flag", 1);
    cJSON *flag = cJSON_GetObjectItem(root, "flag");

    // step 2: Setup (Configure)
    cJSON_AddItemToArray(arr, bool_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate and Validate (Operate → Validate)
    cJSON *arr_first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_bool arr_first_is_bool = cJSON_IsBool(arr_first);
    char *new_val = cJSON_SetValuestring(str_item, "updated");
    int usage_sink = (new_val != NULL) + (flag_is_bool != 0) + (arr_first_is_bool != 0);

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)usage_sink;
    (void)added_flag;
    return 66;
}