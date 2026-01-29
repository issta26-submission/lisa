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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *title = cJSON_CreateString("ExampleTitle");
    cJSON_AddItemToObject(root, "title", title);
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON *count = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "count", count);

    // step 2: Configure
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(extra, "active", active);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    const char *title_str = cJSON_GetStringValue(title_item);
    char status_buf[512];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "title=%s|snapshot_len=%d", title_str ? title_str : "null", snapshot ? (int)strlen(snapshot) : 0);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *final_snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_free(final_snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}