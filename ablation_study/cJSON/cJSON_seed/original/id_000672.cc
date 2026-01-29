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
//<ID> 672
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", flag);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num);

    // step 2: Configure
    int sz = cJSON_GetArraySize(items);
    cJSON *size_num = cJSON_CreateNumber((double)sz);
    cJSON_AddItemToObject(meta, "items_count", size_num);
    cJSON_bool status_was_bool = cJSON_IsBool(flag);
    cJSON_AddBoolToObject(meta, "status_was_bool", status_was_bool);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "status", replacement);
    (void)replaced;
    cJSON *new_status = cJSON_GetObjectItem(root, "status");
    cJSON_bool status_is_string = cJSON_IsString(new_status);
    cJSON_AddBoolToObject(meta, "status_is_string", status_is_string);

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}