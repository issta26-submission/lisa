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
//<ID> 911
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\": {\"name\":\"example\"}, \"items\": []}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *numbers = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateNumber(30.0));
    cJSON_AddItemToArray(numbers, cJSON_CreateString(name_str));

    // step 3: Operate
    cJSON *detached_items = cJSON_DetachItemFromObject(root, "items");
    cJSON_AddItemToArray(numbers, detached_items);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    char *prebuf = (char *)buf;
    cJSON_bool ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}