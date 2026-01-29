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
//<ID> 194
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
    cJSON *null1 = (cJSON *)0;
    cJSON *null2 = (cJSON *)0;
    cJSON *null3 = (cJSON *)0;
    cJSON *size_item = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    char *json_text = (char *)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null1 = cJSON_CreateNull();
    null2 = cJSON_CreateNull();
    null3 = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(arr, null1);
    cJSON_AddItemToArray(arr, null2);
    cJSON_AddItemToArray(arr, null3);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 1);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "remaining_count", size_item);
    first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_AddItemToObject(root, "first_is_null", cJSON_CreateString(cJSON_IsNull(first_item) ? "true" : "false"));
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}