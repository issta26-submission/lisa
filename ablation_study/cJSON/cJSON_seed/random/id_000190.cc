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
//<ID> 190
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
    cJSON *one = (cJSON *)0;
    cJSON *two = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *second_is_null_flag = (cJSON *)0;
    cJSON *first_added_flag = (cJSON *)0;
    cJSON_bool added_array = (cJSON_bool)0;
    cJSON_bool added_first = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    one = cJSON_CreateString("one");
    two = cJSON_CreateString("two");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    added_array = cJSON_AddItemToObject(root, "items", arr);
    added_first = cJSON_AddItemToArray(arr, one);
    cJSON_AddItemToArray(arr, two);
    cJSON_AddItemToArray(arr, null_item);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 1);

    // step 5: Validate
    retrieved = cJSON_GetArrayItem(arr, 1);
    second_is_null_flag = cJSON_CreateString(cJSON_IsNull(retrieved) ? "true" : "false");
    cJSON_AddItemToObject(root, "second_is_null", second_is_null_flag);
    first_added_flag = cJSON_CreateString(added_first ? "true" : "false");
    cJSON_AddItemToObject(root, "first_added", first_added_flag);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}