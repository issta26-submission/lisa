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
//<ID> 134
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
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *tmp = (cJSON *)0;
    cJSON_bool added_null = (cJSON_bool)0;
    cJSON_bool added_num = (cJSON_bool)0;
    cJSON_bool was_null = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    added_null = cJSON_AddItemToArray(arr, null_item);
    added_num = cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate & Validate
    size_before = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromArray(arr, 0);
    was_null = cJSON_IsNull(detached);
    cJSON_AddNumberToObject(root, "detached_was_null", (double)was_null);
    cJSON_AddNumberToObject(root, "array_size_before", (double)size_before);
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size_after", (double)size_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}