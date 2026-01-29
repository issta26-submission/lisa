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
//<ID> 1428
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
    cJSON *enabled_item_created = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", enabled_item_created);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(config, "count", (double)count);

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    size_t ulen = unformatted ? strlen(unformatted) : 0;
    char *scratch = (char *)cJSON_malloc(ulen + 8);
    memset(scratch, 0, (size_t)(ulen + 8));
    scratch[0] = (ulen > 0) ? unformatted[0] : 0;
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + (int)is_enabled);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}