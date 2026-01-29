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
//<ID> 1438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\":{\"a\":1,\"b\":2},\"info\":\"unit\"}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);
    cJSON *config = cJSON_GetObjectItem(root, "config");

    // step 2: Configure
    cJSON *config_dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_dup);
    cJSON_AddRawToObject(root, "raw_snippet", "{\"rawKey\": 42}");
    cJSON *detached_config = cJSON_DetachItemViaPointer(root, config);

    // step 3: Operate & Validate
    const int buffer_length = 512;
    char *buffer = (char *)cJSON_malloc(buffer_length);
    memset(buffer, 0, buffer_length);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buffer_length, 1);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw_snippet");
    const char *raw_string = cJSON_GetStringValue(raw_item);
    size_t actual_len = strlen(buffer);
    (void)printed_ok;
    (void)raw_string;
    (void)actual_len;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_config);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}