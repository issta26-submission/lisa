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
//<ID> 918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"data\": [10, 20], \"meta\": {\"name\":\"original\"}}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *name = cJSON_GetStringValue(name_item);
    cJSON *migrated = cJSON_AddArrayToObject(root, "migrated");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "data");
    cJSON_AddItemToArray(migrated, detached);
    cJSON *extra_num = cJSON_CreateNumber(30);
    cJSON_AddItemToArray(migrated, extra_num);
    cJSON *name_copy = cJSON_CreateString(name);
    cJSON_AddItemToArray(migrated, name_copy);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 256, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}