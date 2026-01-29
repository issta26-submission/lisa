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
//<ID> 1400
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
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON_AddTrueToObject(root, "valid");
    int size_before = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)size_before);
    cJSON_AddItemToObjectCS(root, "count", count_item);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *picked = cJSON_GetArrayItem(arr, 1);
    const char *picked_val = cJSON_GetStringValue(picked);
    cJSON_AddStringToObject(root, "extracted", picked_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}