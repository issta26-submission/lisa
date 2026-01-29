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
//<ID> 1865
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", flag);
    cJSON *params = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "params", params);
    cJSON_AddItemToObject(params, "mode", cJSON_CreateString("initial"));
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(2.0));

    // step 2: Configure
    cJSON *mode_item = cJSON_GetObjectItem(params, "mode");
    char *set_ret = cJSON_SetValuestring(mode_item, "configured");
    (void)set_ret;
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToArray(list, extra);
    cJSON_bool was_true = cJSON_IsTrue(flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(list);
    cJSON *detached = cJSON_DetachItemFromArray(list, 0);
    int size_after = cJSON_GetArraySize(list);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *enabled_item = cJSON_GetObjectItem(got_meta, "enabled");
    cJSON_bool still_true = cJSON_IsTrue(enabled_item);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (size_before % 10));
    buffer[2] = (char)('0' + (size_after % 10));
    buffer[3] = was_true ? 'T' : 'F';
    buffer[4] = still_true ? 't' : 'f';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}