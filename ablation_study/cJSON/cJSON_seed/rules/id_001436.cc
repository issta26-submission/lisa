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
//<ID> 1436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *false_item = NULL;
    cJSON *str_item = NULL;
    cJSON *str_item2 = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    cJSON_bool is_raw_flag = 0;
    char *retrieved_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "array", arr);
    str_item2 = cJSON_CreateString("detached_value");
    cJSON_AddItemToObject(root, "remove_me", str_item2);

    // step 3: Operate / Validate
    is_raw_flag = cJSON_IsRaw(str_item2);
    detached = cJSON_DetachItemViaPointer(root, str_item2);
    retrieved_str = cJSON_GetStringValue(detached);
    (void)is_raw_flag;
    (void)retrieved_str;
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}