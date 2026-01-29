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
//<ID> 139
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
    cJSON *detached = (cJSON *)0;
    cJSON *tmp = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool detached_is_null = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(7.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    tmp = cJSON_AddNumberToObject(root, "initial_count", (double)cJSON_GetArraySize(arr));

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromArray(arr, 0);
    detached_is_null = cJSON_IsNull(detached);
    tmp = cJSON_AddNumberToObject(root, "detached_was_null", (double)detached_is_null);

    // step 5: Validate
    size_after = cJSON_GetArraySize(arr);
    tmp = cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}