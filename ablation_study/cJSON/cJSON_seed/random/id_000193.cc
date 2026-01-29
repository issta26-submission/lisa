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
//<ID> 193
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
    cJSON *null_in_array = (cJSON *)0;
    cJSON *null_in_object = (cJSON *)0;
    cJSON_bool added_array_to_root = (cJSON_bool)0;
    cJSON_bool added_null_to_array = (cJSON_bool)0;
    cJSON_bool added_null_to_object = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null_in_array = cJSON_CreateNull();
    null_in_object = cJSON_CreateNull();

    // step 3: Configure
    added_array_to_root = cJSON_AddItemToObject(root, "data", arr);
    added_null_to_array = cJSON_AddItemToArray(arr, null_in_array);
    added_null_to_object = cJSON_AddItemToObject(root, "nullable", null_in_object);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 0);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}