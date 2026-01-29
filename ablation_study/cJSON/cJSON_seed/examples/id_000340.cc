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
//<ID> 340
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawKey\":true}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    int size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)size);

    // step 3: Operate & Validate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}