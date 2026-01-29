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
//<ID> 195
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(items, raw);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, s1);
    cJSON *greeting = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate/Validate
    cJSON_bool is_raw = cJSON_IsRaw(raw);
    int arr_size = cJSON_GetArraySize(items);
    cJSON *count = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *raw_flag = cJSON_CreateBool(is_raw);
    cJSON_AddItemToObject(root, "raw_flag", raw_flag);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}