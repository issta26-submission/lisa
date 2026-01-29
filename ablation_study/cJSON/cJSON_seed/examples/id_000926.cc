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
//<ID> 926
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
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON *str_orig = cJSON_CreateString("old-name");
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", str_orig);

    // step 2: Configure
    cJSON *picked_num = cJSON_GetArrayItem(arr, 1);
    cJSON_SetNumberHelper(picked_num, 42.5);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("new-name");
    cJSON_ReplaceItemInObject(root, "name", replacement);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *validated_num = cJSON_GetArrayItem(got_items, 1);
    cJSON_SetNumberHelper(validated_num, cJSON_SetNumberHelper(validated_num, 0.0) + 0.0);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}