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
//<ID> 2294
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled_false);
    cJSON_AddStringToObject(config, "name", "device-A");

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "id", "m-01");
    cJSON_AddItemToArray(items, meta);

    // step 3: Operate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON_AddItemToObject(root, "has_config", cJSON_CreateBool(has_cfg));
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)count);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}