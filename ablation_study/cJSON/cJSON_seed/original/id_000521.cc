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
//<ID> 521
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"device\":\"Sensor\",\"status\":\"idle\",\"metrics\":{\"temp\":23}}";
    cJSON *root = cJSON_Parse(initial_json);
    cJSON *tag_item = cJSON_CreateString("alpha");
    cJSON_AddItemToObjectCS(root, "tag", tag_item);
    cJSON *id_item = cJSON_CreateNumber(1001.0);
    cJSON_AddItemToObjectCS(root, "id", id_item);

    // step 2: Configure
    cJSON *status_item = cJSON_DetachItemFromObject(root, "status");
    cJSON_SetValuestring(status_item, "active");
    cJSON_AddItemToObjectCS(root, "status", status_item);
    cJSON *extra = cJSON_Parse("{\"extra\":true}");
    cJSON_AddItemToObjectCS(root, "extra_info", extra);

    // step 3: Operate
    cJSON_SetValuestring(cJSON_GetObjectItem(root, "tag"), "beta");
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    const char *status_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "status"));
    double temp_val = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "metrics"), "temp"));
    (void)status_str;
    (void)temp_val;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}