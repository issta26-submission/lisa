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
//<ID> 1387
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddItemToObject(root, "greeting", str);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    int sz = cJSON_GetArraySize(arr);
    cJSON_SetNumberHelper(num, (double)sz + 3.14);
    const char *text = cJSON_GetStringValue(str);
    cJSON *copy = cJSON_CreateString(text);
    cJSON_AddItemToObject(root, "greeting_copy", copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}