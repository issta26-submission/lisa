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
//<ID> 328
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
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double detached_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    str_item = cJSON_CreateString("sample");
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate
    is_array = cJSON_IsArray(arr);
    detached = cJSON_DetachItemViaPointer(root, num_item);
    detached_value = cJSON_GetNumberValue(detached);

    // step 5: Validate / modify
    cJSON_AddNumberToObject(root, "detached_value", detached_value);
    cJSON_AddNumberToObject(root, "array_flag", (double)is_array);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}