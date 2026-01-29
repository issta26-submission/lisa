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
//<ID> 1404
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
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    int size = cJSON_GetArraySize(arr);
    int idx = size - 1;
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInArray(arr, idx, replacement);

    // step 3: Operate & Validate
    cJSON *flag_cs = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "flagCS", flag_cs);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot_raw", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}