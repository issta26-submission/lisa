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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON *enabled_item = cJSON_AddTrueToObject(config, "enabled");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "id", "record-001");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *got_enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(got_enabled);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = strlen(unfmt);
    size_t alloc = len + 16;
    char *scratch = (char *)cJSON_malloc(alloc);
    memset(scratch, 0, alloc);
    scratch[0] = unfmt[0];
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + (int)enabled_flag);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}