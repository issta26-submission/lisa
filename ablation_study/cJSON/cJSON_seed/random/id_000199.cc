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
//<ID> 199
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
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    char *json_text = (char *)0;
    int remaining = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    null_item = cJSON_CreateNull();
    str_item = cJSON_CreateString("to_be_deleted");
    num_item = cJSON_CreateNumber(7.0);

    // step 4: Operate
    add_ok1 = cJSON_AddItemToArray(arr, null_item);
    add_ok2 = cJSON_AddItemToArray(arr, str_item);
    add_ok3 = cJSON_AddItemToArray(arr, num_item);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 5: Validate
    remaining = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)remaining);
    cJSON_AddItemToObject(root, "remaining_count", count_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}