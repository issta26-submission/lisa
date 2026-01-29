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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateBool(1);
    cJSON *item_b = cJSON_CreateBool(0);
    cJSON *item_c = cJSON_CreateBool(1);
    cJSON *detached = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool moved_is_bool = 0;

    // step 2: Setup
    cJSON_AddTrueToObject(root_obj, "explicit_true");
    cJSON_AddItemToObjectCS(root_obj, "object_false", item_b);
    cJSON_AddItemToArray(root_array, item_a);
    cJSON_AddItemToArray(root_array, item_c);

    // step 3: Core operations
    detached = cJSON_DetachItemFromArray(root_array, 1);
    cJSON_AddItemToObjectCS(root_obj, "moved_from_array", detached);

    // step 4: Validate
    arr_size = cJSON_GetArraySize(root_array);
    moved_is_bool = cJSON_IsBool(cJSON_GetObjectItem(root_obj, "moved_from_array"));

    // step 5: Cleanup
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}