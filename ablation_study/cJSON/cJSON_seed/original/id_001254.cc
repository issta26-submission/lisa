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
//<ID> 1254
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

    // step 2: Configure
    cJSON_AddStringToObject(config, "mode", "fast");
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", config_ref);

    // step 3: Operate
    cJSON_bool invalid_flag = cJSON_IsInvalid(config_ref);
    cJSON_AddNumberToObject(root, "invalid_flag", (double)invalid_flag);
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_ref = cJSON_GetObjectItem(root, "config_ref");
    cJSON *detached = cJSON_DetachItemViaPointer(root, got_config);
    cJSON_AddItemToObject(root, "restored", detached);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    int buf_len = (int)(printed_len + 32);
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);

    // step 4: Validate & Cleanup
    cJSON *restored = cJSON_GetObjectItem(root, "restored");
    cJSON *mode_item = cJSON_GetObjectItem(restored, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    (void)mode_str;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}