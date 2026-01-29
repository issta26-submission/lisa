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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, num1);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str1);
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddStringToObject(sub, "mode", "fast");
    cJSON_AddItemToObject(config, "sub", sub);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_sub = cJSON_GetObjectItem(got_config, "sub");
    cJSON_bool sub_is_invalid = cJSON_IsInvalid(got_sub);
    (void)sub_is_invalid;
    cJSON *detached = cJSON_DetachItemViaPointer(got_config, got_sub);
    cJSON_AddItemToObject(root, "orphan", detached);
    char *unformatted = cJSON_PrintUnformatted(root);
    size_t unformatted_len = strlen(unformatted);
    int pre_len = (int)(unformatted_len + 16);
    char *prebuf = (char *)cJSON_malloc((size_t)pre_len);
    memset(prebuf, 0, (size_t)pre_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, prebuf, pre_len, 0);
    (void)printed_ok;

    // step 4: Validate & Cleanup
    cJSON *orphan = cJSON_GetObjectItem(root, "orphan");
    cJSON *mode_item = cJSON_GetObjectItem(orphan, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    (void)mode_str;
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}