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
//<ID> 1407
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
    cJSON *num1 = cJSON_CreateNumber(2.5);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObjectCS(root, "list", arr);

    // step 2: Configure
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(arr, 1, replacement_true);
    cJSON_AddTrueToObject(root, "explicit_flag");

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(arr);
    double current = cJSON_GetNumberValue(num1);
    cJSON_SetNumberHelper(num1, current + (double)arr_size);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "snapshot_str", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}