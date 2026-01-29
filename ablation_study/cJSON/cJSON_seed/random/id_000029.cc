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
//<ID> 29
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
    cJSON *item_one = cJSON_CreateString("one");
    cJSON *item_shared = cJSON_CreateString("shared");
    cJSON *detached_item = (cJSON *)0;
    char json_buf[] = "  { \"hello\" : \"world\" }  ";
    int arr_size = 0;
    cJSON_bool invalid_flag = 0;

    // step 2: Setup
    cJSON_AddItemToArray(root_array, item_one);
    cJSON_AddItemToArray(root_array, item_shared);
    cJSON_AddItemReferenceToObject(root_obj, "myref", item_shared);

    // step 3: Core operations
    detached_item = cJSON_DetachItemFromArray(root_array, 1);
    invalid_flag = cJSON_IsInvalid(detached_item);
    cJSON_Minify(json_buf);
    arr_size = cJSON_GetArraySize(root_array);

    // step 4: Configure
    cJSON_AddNumberToObject(root_obj, "size", (double)arr_size);
    cJSON_AddBoolToObject(root_obj, "was_invalid", invalid_flag);

    // step 5: Validate
    cJSON_DeleteItemFromObject(root_obj, "myref");

    // step 6: Cleanup
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}