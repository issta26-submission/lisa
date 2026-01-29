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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_shared = cJSON_CreateString("shared");
    cJSON *detached = (cJSON *)0;
    cJSON *size_num = (cJSON *)0;
    char json_buf[] = "  { \"key\" : \"value\" }  ";
    int arr_size = 0;
    cJSON_bool invalid_check = 0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_shared);
    cJSON_AddItemReferenceToObject(root, "myref", item_shared);
    cJSON_AddItemToObject(root, "array", arr);

    // step 3: Core operations
    detached = cJSON_DetachItemFromArray(arr, 1);
    arr_size = cJSON_GetArraySize(arr);
    size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "size", size_num);
    invalid_check = cJSON_IsInvalid(detached);
    cJSON_SetValuestring(detached, "modified_shared");
    cJSON_Minify(json_buf);

    // step 4: Validate
    cJSON_DeleteItemFromObject(root, "myref");

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}