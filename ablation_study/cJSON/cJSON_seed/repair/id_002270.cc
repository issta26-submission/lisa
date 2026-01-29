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
//<ID> 2270
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
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON *name = cJSON_CreateString("deviceXYZ");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *count = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *raw_payload = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw", raw_payload);

    // step 2: Configure
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", config_ref);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "2.4");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)raw_flag);
    buffer[1] = (char)('0' + (int)printed);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}