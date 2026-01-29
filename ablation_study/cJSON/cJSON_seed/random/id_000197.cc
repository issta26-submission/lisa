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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *second_null = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool first_is_null = (cJSON_bool)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    second_null = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, second_null);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON_AddItemToArray(arr, cJSON_CreateNull());

    // step 5: Validate
    first_item = cJSON_GetArrayItem(arr, 0);
    first_is_null = cJSON_IsNull(first_item);
    flag_item = cJSON_AddBoolToObject(root, "first_is_null", first_is_null);
    arr_size = cJSON_GetArraySize(arr);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}