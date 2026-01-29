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
//<ID> 522
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"device\":{\"id\":\"dev1\",\"status\":\"ok\"},\"meta\":\"init\"}";
    cJSON *parsed = cJSON_Parse(initial_json);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "imported", parsed);
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObjectCS(root, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObjectCS(root, "count", count_item);

    // step 2: Configure
    cJSON_SetValuestring(name_item, "updated_name");
    cJSON_SetValuestring(cJSON_GetObjectItem(parsed, "meta"), "configured");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", enabled);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "imported");
    cJSON_SetValuestring(cJSON_GetObjectItem(cJSON_GetObjectItem(detached, "device"), "id"), "devX");
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    const char *name_after = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    double count_after = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    const char *detached_id = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(detached, "device"), "id"));
    (void)name_after;
    (void)count_after;
    (void)detached_id;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}