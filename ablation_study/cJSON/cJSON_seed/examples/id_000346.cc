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
//<ID> 346
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
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *null_item = cJSON_AddNullToObject(root, "maybe");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "greeting", s2);
    cJSON *count_num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    double updated = cJSON_SetNumberHelper(count_num, (double)size);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}