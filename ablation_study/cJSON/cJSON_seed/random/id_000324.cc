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
//<ID> 324
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
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double num_value = 0.0;
    cJSON_bool is_arr = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    num_item = cJSON_AddNumberToObject(root, "count", 123.5);

    // step 4: Operate
    is_arr = cJSON_IsArray(arr);
    num_value = cJSON_GetNumberValue(num_item);
    detached = cJSON_DetachItemViaPointer(arr, str_item);

    // step 5: Validate
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_AddNumberToObject(root, "double_count", num_value * 2.0);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}