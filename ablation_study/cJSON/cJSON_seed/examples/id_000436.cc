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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_version = "1.0.0";
    const char *initial_mode = "active";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *version_item = cJSON_CreateString(initial_version);
    cJSON_AddItemToObject(meta, "version", version_item);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *mode_item = cJSON_CreateString(initial_mode);
    cJSON_AddItemToObject(settings, "mode", mode_item);
    cJSON *retry_item = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(settings, "retry", retry_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    const cJSON *meta_obj = cJSON_GetObjectItem(root, "meta");
    const cJSON *version_obj = cJSON_GetObjectItem((cJSON*)meta_obj, "version");
    const char *version_str = cJSON_GetStringValue(version_obj);
    const cJSON *settings_obj = cJSON_GetObjectItem(root, "settings");
    const cJSON *mode_obj = cJSON_GetObjectItem((cJSON*)settings_obj, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_obj);
    double retry_val = cJSON_GetNumberValue(cJSON_GetObjectItem((cJSON*)settings_obj, "retry"));
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "version=%s|mode=%s|retry=%.0f|snapshot_len=%d",
            version_str ? version_str : "null",
            mode_str ? mode_str : "null",
            retry_val,
            snapshot ? (int)strlen(snapshot) : 0);
    cJSON_AddStringToObject(root, "status", status_buf);
    cJSON *snapshot_str = cJSON_CreateString(snapshot ? snapshot : "");
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}