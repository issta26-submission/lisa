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
//<ID> 89
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("elem");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_bool arr_is_array_before = cJSON_IsArray(arr);
    cJSON *detached = cJSON_DetachItemViaPointer(root, arr);
    cJSON_bool detached_is_array = cJSON_IsArray(detached);
    int summary = (int)arr_is_array_before + (int)detached_is_array + (int)(printed ? printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    if (printed) cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}