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
//<ID> 1403
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
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *str = cJSON_CreateString("alpha");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *replacement = cJSON_CreateNumber((double)size + 3.5);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    double newval = (double)size + (double)cJSON_IsTrue(flag);
    cJSON_SetNumberHelper(num1, newval);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}