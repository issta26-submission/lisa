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
//<ID> 505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("ExampleUser");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"meta\":\"rawdata\"}");
    cJSON_AddItemToObject(root, "metadata_raw", raw_item);

    // step 2: Configure
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    cJSON_bool metadata_is_raw = cJSON_IsRaw(raw_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 3: Operate
    size_t buf_len = (size_t)128 + (size_t)enabled_is_true + (size_t)metadata_is_raw;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    (void)name_val;
    (void)enabled_is_true;
    (void)metadata_is_raw;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}