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
//<ID> 137
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
    cJSON *nul = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool is_detached_null = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(3.14);
    nul = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, nul);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromArray(arr, 1);
    is_detached_null = cJSON_IsNull(detached);
    cJSON_AddNumberToObject(root, "summary", (double)arr_size + (double)is_detached_null);

    // step 5: Validate
    json_text = cJSON_Print(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}