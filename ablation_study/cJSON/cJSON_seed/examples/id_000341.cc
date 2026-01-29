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
//<ID> 341
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
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "words", arr);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    char numbuf[32];
    memset(numbuf, 0, sizeof(numbuf));
    snprintf(numbuf, sizeof(numbuf), "%d", count);
    cJSON *count_str = cJSON_CreateString(numbuf);
    cJSON_AddItemToObject(root, "count", count_str);

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}