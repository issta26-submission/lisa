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
//<ID> 1734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddBoolToObject(cfg, "enabled", (cJSON_bool)1);
    cJSON *name_item = cJSON_CreateString("device42");
    cJSON_AddItemToObject(cfg, "name", name_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawKey\":123}");
    cJSON_AddItemToObject(cfg, "raw", raw_item);

    // step 3: Operate and Validate
    cJSON *retr_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *retr_name = cJSON_GetObjectItem(retr_cfg, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_enabled = cJSON_GetObjectItem(retr_cfg, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(retr_enabled);
    cJSON *retr_raw = cJSON_GetObjectItem(retr_cfg, "raw");
    const char *raw_val = cJSON_GetStringValue(retr_raw);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + enabled_true);
    buffer[3] = raw_val ? raw_val[0] : 'Z';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}