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
//<ID> 2415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, num);
    cJSON_AddItemToArray(items, str);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "title", "original");

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_items = cJSON_GetObjectItem(dup, "items");
    cJSON *second = cJSON_GetArrayItem(dup_items, 1);
    char *before = cJSON_GetStringValue(second);
    char *newval = cJSON_SetValuestring(second, "world");
    buffer[1] = before[0];
    buffer[2] = newval[0];

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(dup);
    buffer[3] = printed2[0];
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}