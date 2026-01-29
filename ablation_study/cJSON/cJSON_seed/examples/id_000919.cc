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
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("device-1");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate & Validate
    cJSON *got_str = cJSON_GetArrayItem(arr, 0);
    cJSON *got_num = cJSON_GetArrayItem(arr, 2);
    cJSON_SetNumberHelper(got_num, 42.5);
    cJSON *str2 = cJSON_CreateString("device-1");
    cJSON_bool are_equal = cJSON_Compare(got_str, str2, 1);
    cJSON_AddBoolToObject(root, "are_equal", are_equal);
    cJSON_Delete(str2);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}