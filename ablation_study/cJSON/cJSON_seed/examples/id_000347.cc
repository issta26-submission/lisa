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
//<ID> 347
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
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *raw_item = cJSON_CreateRaw("\"raw_value\"");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)count);

    // step 3: Operate & Validate
    char buffer[256];
    buffer[0] = '\0';
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}