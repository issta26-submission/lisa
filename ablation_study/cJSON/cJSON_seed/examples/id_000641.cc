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
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, nul);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, nul, replacement);

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    int count = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    double second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;first=%s;second=%g;json=%s",
            version ? version : "null",
            count,
            first_str ? first_str : "null",
            second_val,
            buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)status;
    // API sequence test completed successfully
    return 66;
}