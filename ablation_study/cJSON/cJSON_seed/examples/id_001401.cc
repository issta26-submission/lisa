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
//<ID> 1401
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObjectCS(root, "items", arr);
    cJSON *enabled_ptr = cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddItemToObjectCS(root, "meta", cJSON_CreateObject());

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *replacement = cJSON_CreateFalse();
    cJSON_ReplaceItemInArray(arr, size - 1, replacement);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}