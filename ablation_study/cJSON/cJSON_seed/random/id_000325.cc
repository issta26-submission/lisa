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
//<ID> 325
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
    double numeric_value = 0.0;
    cJSON_bool arr_check = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "arr", arr);
    str_item = cJSON_CreateString("detachable_string");
    cJSON_AddItemToArray(arr, str_item);
    num_item = cJSON_AddNumberToObject(root, "count", 123.456);

    // step 4: Operate
    arr_check = cJSON_IsArray(cJSON_GetObjectItem(root, "arr"));
    numeric_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    detached = cJSON_DetachItemViaPointer(arr, str_item);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 5: Validate
    numeric_value = cJSON_GetNumberValue(num_item);
    (void)arr_check;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}