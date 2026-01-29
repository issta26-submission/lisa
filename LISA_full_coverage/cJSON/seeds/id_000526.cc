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
//<ID> 526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"device\":\"Sensor\",\"status\":\"idle\",\"metrics\":[1,2,3]}";
    cJSON *root = cJSON_Parse(initial_json);
    cJSON *status_item = cJSON_DetachItemFromObject(root, "status");
    char *status_before = cJSON_GetStringValue(status_item);

    // step 2: Configure
    cJSON_SetValuestring(status_item, "active");
    cJSON_bool add_status_result = cJSON_AddItemToObjectCS(root, "status", status_item);

    // step 3: Operate
    cJSON *owner_item = cJSON_Parse("\"Alice\"");
    cJSON_AddItemToObjectCS(root, "owner", owner_item);
    cJSON_SetValuestring(owner_item, "Bob");
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 16;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    const char *device_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "device"));
    int metrics_count = cJSON_GetArraySize(cJSON_GetObjectItem(root, "metrics"));
    (void)status_before;
    (void)add_status_result;
    (void)device_name;
    (void)metrics_count;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}