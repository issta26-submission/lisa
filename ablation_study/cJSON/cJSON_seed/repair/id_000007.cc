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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations and initialization
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_true = cJSON_CreateTrue();
    cJSON *item_false = cJSON_CreateFalse();
    cJSON *str_item = cJSON_CreateString("initial");
    // step 2: Setup - build array and attach to root
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "array", arr);
    // step 3: Operate and validate
    cJSON_SetValuestring(str_item, "updated");
    cJSON_bool is_true = cJSON_IsBool(item_true);
    cJSON_bool is_false = cJSON_IsBool(item_false);
    cJSON_AddBoolToObject(root, "first_is_bool", is_true);
    cJSON_AddBoolToObject(root, "second_is_bool", is_false);
    cJSON_DeleteItemFromArray(arr, 1);
    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}