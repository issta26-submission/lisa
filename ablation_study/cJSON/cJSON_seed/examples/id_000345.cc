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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);
    cJSON *raw1 = cJSON_CreateRaw("\"raw_value\"");
    cJSON_AddItemToArray(arr, raw1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 3: Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "items_count", count_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}