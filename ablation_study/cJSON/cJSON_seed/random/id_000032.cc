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
//<ID> 32
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
    cJSON *bool_item_false = cJSON_CreateBool(0);
    cJSON *bool_item_true = cJSON_CreateBool(1);
    cJSON *added_true = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    int arr_size_before = 0;
    int arr_size_after = 0;
    char *printed = (char *)0;

    // step 2: Setup
    added_true = cJSON_AddTrueToObject(root_obj, "always_true");
    cJSON_AddItemToArray(root_array, bool_item_false);
    cJSON_AddItemToArray(root_array, bool_item_true);
    cJSON_AddItemToObjectCS(root_obj, "my_array", root_array);

    // step 3: Core operations
    arr_size_before = cJSON_GetArraySize(root_array);
    detached_item = cJSON_DetachItemFromArray(root_array, 1);
    cJSON_AddItemToObjectCS(root_obj, "detached_item", detached_item);
    arr_size_after = cJSON_GetArraySize(root_array);

    // step 4: Validate
    printed = cJSON_PrintUnformatted(root_obj);
    (void)arr_size_before;
    (void)arr_size_after;
    cJSON_free(printed);

    // step 5: Cleanup
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}