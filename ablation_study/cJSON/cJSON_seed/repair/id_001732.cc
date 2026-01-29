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
//<ID> 1732
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *desc = cJSON_CreateString("example");
    cJSON_AddItemToObject(info, "desc", desc);
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON *enabled = cJSON_AddBoolToObject(info, "enabled", 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "note", meta_note);

    // step 3: Operate and Validate
    cJSON *retr_info = cJSON_GetObjectItem(root, "info");
    cJSON *retr_desc = cJSON_GetObjectItem(retr_info, "desc");
    char *desc_val = cJSON_GetStringValue(retr_desc);
    cJSON *retr_enabled = cJSON_GetObjectItem(retr_info, "enabled");
    int enabled_flag = cJSON_IsTrue(retr_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = desc_val ? desc_val[0] : 'Y';
    buffer[2] = (char)('0' + (enabled_flag ? 1 : 0));
    (void)buffer;
    (void)enabled_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}