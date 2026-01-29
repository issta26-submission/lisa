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
//<ID> 133
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool was_null = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    num2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddNumberToObject(root, "init_marker", 1.0);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromArray(arr, 1);
    was_null = cJSON_IsNull(detached);
    cJSON_AddNumberToObject(root, "was_null", (double)was_null);

    // step 5: Validate
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}