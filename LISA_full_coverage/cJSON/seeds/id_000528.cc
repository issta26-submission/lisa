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
//<ID> 528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"device\":\"DeviceA\",\"count\":7}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *extra = cJSON_CreateString("initial");
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 2: Configure
    cJSON *device = cJSON_GetObjectItem(root, "device");
    cJSON_SetValuestring(device, "DeviceB");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");

    // step 3: Operate
    cJSON *detached_count = cJSON_DetachItemFromObject(root, "count");
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool extra_present = cJSON_HasObjectItem(root, "extra");
    char *device_value = cJSON_GetStringValue(device);

    // step 4: Validate & Cleanup
    (void)extra_present;
    (void)device_value;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached_count);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}