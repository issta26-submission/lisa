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
//<ID> 322
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
    double extracted = 0.0;
    cJSON_bool is_array = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("example_string");
    num_item = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate
    cJSON_AddNumberToObject(root, "added_total", 99.5);
    extracted = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    is_array = cJSON_IsArray(arr);

    // step 5: Validate
    detached = cJSON_DetachItemViaPointer(arr, str_item);
    cJSON_AddItemToObject(root, "moved_string", detached);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}