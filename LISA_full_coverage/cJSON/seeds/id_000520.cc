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
//<ID> 520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"device\":\"Sensor\",\"value\":42}";
    cJSON *root = cJSON_Parse(initial_json);
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObjectCS(root, "status", status);
    cJSON *meta = cJSON_CreateObject();
    cJSON *ver = cJSON_CreateString("v1.0");
    cJSON_AddItemToObjectCS(meta, "version", ver);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *device_item = cJSON_GetObjectItem(root, "device");
    char *set_ret = cJSON_SetValuestring(device_item, "DeviceX");
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON *meta_version = cJSON_GetObjectItem(detached_meta, "version");
    char *set_meta = cJSON_SetValuestring(meta_version, "v1.1");
    cJSON *additional = cJSON_Parse("{\"uptime\":1234,\"status\":\"running\"}");

    // step 3: Operate
    cJSON_AddItemToObjectCS(root, "additional", additional);
    cJSON_AddItemToObjectCS(root, "info", detached_meta);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 16;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double value_extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    char *device_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "device"));
    (void)set_ret;
    (void)set_meta;
    (void)value_extracted;
    (void)device_str;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}