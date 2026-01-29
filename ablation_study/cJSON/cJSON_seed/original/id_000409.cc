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
//<ID> 409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *replace_target = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "replaceme", replace_target);

    // step 2: Configure
    cJSON *n3 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, n3);
    cJSON *item_index_1 = cJSON_GetArrayItem(arr, 1);
    double val1 = cJSON_GetNumberValue(item_index_1);
    cJSON_bool invalid_before = cJSON_IsInvalid(item_index_1);
    cJSON *flag_before = cJSON_CreateBool(invalid_before ? 1 : 0);
    cJSON_AddItemToObject(root, "was_invalid", flag_before);
    cJSON *replacement = cJSON_CreateNumber(val1 * 10.0);

    // step 3: Operate
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replaceme", replacement);
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *new_item_at0 = cJSON_GetArrayItem(arr, 0);
    double new_val = cJSON_GetNumberValue(new_item_at0);
    cJSON *scaled = cJSON_CreateNumber(new_val + 0.5);
    cJSON_AddItemToObject(root, "scaled_first", scaled);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON_bool invalid_after = cJSON_IsInvalid(new_item_at0);
    cJSON *flag_after = cJSON_CreateBool(invalid_after ? 1 : 0);
    cJSON_AddItemToObject(root, "is_invalid_now", flag_after);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}