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
//<ID> 924
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
    cJSON *str0 = cJSON_CreateString("alpha");
    cJSON *count = cJSON_CreateNumber(3.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    cJSON *arr_item = cJSON_GetArrayItem(arr, 1);
    cJSON_SetNumberHelper(arr_item, 42.5);
    cJSON *replacement = cJSON_CreateString("replaced-count");
    cJSON_ReplaceItemInObject(root, "count", replacement);
    cJSON *got_count = cJSON_GetObjectItem(root, "count");

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)got_count;
    return 66;
    // API sequence test completed successfully
}