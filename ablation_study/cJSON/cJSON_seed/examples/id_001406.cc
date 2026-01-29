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
//<ID> 1406
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObjectCS(root, "data", arr);
    cJSON_AddTrueToObject(root, "flag");

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObjectCS(root, "snapshot", cJSON_CreateString(snapshot));
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}