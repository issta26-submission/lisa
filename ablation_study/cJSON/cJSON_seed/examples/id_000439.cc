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
//<ID> 439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *app_name = "example_app";
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *name_item = cJSON_CreateString(app_name);
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *version_item = cJSON_CreateNumber(1.23);
    cJSON_AddItemToObject(config, "version", version_item);
    cJSON *enabled_item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(config, "enabled", enabled_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *author_item = cJSON_CreateString("dev_team");
    cJSON_AddItemToObject(meta, "author", author_item);
    cJSON *license_item = cJSON_CreateString("MIT");
    cJSON_AddItemToObject(meta, "license", license_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    const char *retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "config"), "name"));
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "app=%s", retrieved_name);
    cJSON_AddStringToObject(root, "status", status_buf);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}