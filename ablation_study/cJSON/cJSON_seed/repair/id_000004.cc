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
//<ID> 4
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
    cJSON *bool_true = cJSON_CreateTrue();
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON *str_item = cJSON_CreateString("initial");

    // step 2: Setup (Configure)
    cJSON_bool added1 = cJSON_AddItemToArray(arr, bool_true);
    cJSON_bool added2 = cJSON_AddItemToArray(arr, bool_false);

    // step 3: Operate and Validate (Operate → Validate)
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_bool is_bool = cJSON_IsBool(bool_false);
    cJSON *added_bool_obj = cJSON_AddBoolToObject(root, "active", is_bool);
    char *newstr = cJSON_SetValuestring(str_item, "updated");
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", str_item);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}