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
//<ID> 2272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "enabled", cJSON_CreateTrue());
    cJSON_AddStringToObject(config, "name", "device-xyz");
    cJSON *raw_node = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(config, "raw", raw_node);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "timestamp", 1627846267.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "2.1");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_raw = cJSON_GetObjectItem(got_config, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)raw_flag);
    buffer[1] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}