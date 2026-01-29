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
//<ID> 21
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
    cJSON *item_shared = cJSON_CreateString("shared_array");
    cJSON *item_other = cJSON_CreateString("shared_object");
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *null_item = cJSON_CreateNull();
    char json_buf[] = "  [ \"a\" , 123 , true ]  ";
    cJSON_bool invalid_flag = cJSON_IsInvalid(null_item);

    // step 2: Setup
    cJSON_AddItemToArray(root_array, item_shared);
    cJSON_AddItemToArray(root_array, num_item);
    cJSON_AddItemReferenceToObject(root_obj, "alias", item_other);
    cJSON_AddItemToObject(root_obj, "flag", cJSON_CreateBool(invalid_flag));

    // step 3: Core operations
    int arr_size = cJSON_GetArraySize(root_array);
    cJSON_Minify(json_buf);

    // step 4: Validate
    cJSON_AddNumberToObject(root_obj, "size", (double)arr_size);

    // step 5: Cleanup
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);
    cJSON_Delete(null_item);

    // API sequence test completed successfully
    return 66;
}