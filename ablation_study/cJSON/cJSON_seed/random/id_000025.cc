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
//<ID> 25
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
    cJSON *item_one = cJSON_CreateString("alpha");
    cJSON *item_shared = cJSON_CreateString("shared");
    int array_size = 0;
    cJSON_bool invalid_flag = 0;
    char json_buf[] = "  { \"trim\" : \"me\" }  ";

    // step 2: Setup
    cJSON_AddItemToArray(root_array, item_one);
    cJSON_AddItemToArray(root_array, item_shared);
    cJSON_AddItemReferenceToObject(root_obj, "shared_ref", item_shared);

    // step 3: Operate
    array_size = cJSON_GetArraySize(root_array);
    invalid_flag = cJSON_IsInvalid(item_one);
    cJSON_Minify(json_buf);

    // step 4: Validate
    cJSON_DeleteItemFromObject(root_obj, "shared_ref");

    // step 5: Cleanup
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}