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
//<ID> 525
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
    cJSON *device_item = cJSON_GetObjectItem(root, "device");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON *status_item = cJSON_CreateString("ok");
    cJSON_bool add_status_ok = cJSON_AddItemToObjectCS(root, "status", status_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON *unit_item = cJSON_CreateString("C");
    cJSON_bool add_unit_ok = cJSON_AddItemToObjectCS(meta, "unit", unit_item);
    cJSON_bool add_meta_ok = cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    char *old_device_str = cJSON_SetValuestring(device_item, "Sensor42");
    cJSON *detached_value = cJSON_DetachItemFromObject(root, "value");
    cJSON *new_value = cJSON_CreateNumber(100.0);
    cJSON_bool replace_value_ok = cJSON_AddItemToObjectCS(root, "value", new_value);
    cJSON_bool readd_old_ok = cJSON_AddItemToObjectCS(root, "old_value", detached_value);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *detached_set = cJSON_SetValuestring(detached_value, "42"); // update detached content before it was readded (safe even if already readded)

    // step 4: Validate & Cleanup
    const char *extracted_device = cJSON_GetStringValue(device_item);
    double extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)add_status_ok;
    (void)add_unit_ok;
    (void)add_meta_ok;
    (void)old_device_str;
    (void)replace_value_ok;
    (void)readd_old_ok;
    (void)snaplen;
    (void)buffer;
    (void)detached_set;
    (void)extracted_device;
    (void)extracted_value;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}